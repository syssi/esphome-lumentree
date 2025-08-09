#include "lumentree_ble.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

#ifdef USE_ESP32

namespace esphome {
namespace lumentree_ble {

static const char *const TAG = "lumentree_ble";

static const uint16_t LUMENTREE_SERVICE_UUID = 0xFFE0;
static const uint16_t LUMENTREE_NOTIFY_CHARACTERISTIC_UUID = 0xFFE1;

static const uint8_t LUMENTREE_MODBUS_DEVICE_ADDR = 0x01;
static const uint8_t LUMENTREE_MODBUS_FUNCTION_READ = 0x03;
static const uint8_t LUMENTREE_MODBUS_FUNCTION_READ_INPUT = 0x04;

static const uint8_t MAX_RESPONSE_SIZE = 255;
static const uint32_t FRAME_TIMEOUT_MS = 5000;

void LumentreeBle::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                       esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      ESP_LOGI(TAG, "[%s] Connected", this->parent_->address_str().c_str());
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      ESP_LOGW(TAG, "[%s] Disconnected", this->parent_->address_str().c_str());
      this->frame_buffer_.clear();
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *service = this->parent_->get_service(LUMENTREE_SERVICE_UUID);
      if (service == nullptr) {
        ESP_LOGE(TAG, "[%s] No service found at 0x%04X", this->parent_->address_str().c_str(), LUMENTREE_SERVICE_UUID);
        break;
      }

      auto *ble_char = service->get_characteristic(LUMENTREE_NOTIFY_CHARACTERISTIC_UUID);
      if (ble_char == nullptr) {
        ESP_LOGE(TAG, "[%s] No characteristic found at 0x%04X", this->parent_->address_str().c_str(),
                 LUMENTREE_NOTIFY_CHARACTERISTIC_UUID);
        break;
      }

      this->char_handle_ = ble_char->handle;

      auto status = esp_ble_gattc_register_for_notify(this->parent_->get_gattc_if(), this->parent_->get_remote_bda(),
                                                      ble_char->handle);
      if (status) {
        ESP_LOGW(TAG, "[%s] esp_ble_gattc_register_for_notify failed, status=%d", this->parent_->address_str().c_str(),
                 status);
      }
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = esp32_ble_tracker::ClientState::ESTABLISHED;
      ESP_LOGI(TAG, "[%s] BLE connection established", this->parent_->address_str().c_str());
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      if (param->notify.handle == this->char_handle_) {
        ESP_LOGVV(TAG, "[%s] Notification received: %s", this->parent_->address_str().c_str(),
                  format_hex_pretty(param->notify.value, param->notify.value_len).c_str());
        this->assemble(param->notify.value, param->notify.value_len);
      }
      break;
    }
    default:
      break;
  }
}

void LumentreeBle::assemble(const uint8_t *data, uint16_t length) {
  uint32_t now = millis();

  // Check for frame timeout - clear stale data
  if (!this->frame_buffer_.empty() && (now - this->last_frame_timestamp_) > FRAME_TIMEOUT_MS) {
    ESP_LOGW(TAG, "Frame timeout (%dms), clearing stale buffer (%d bytes)", now - this->last_frame_timestamp_,
             this->frame_buffer_.size());
    this->frame_buffer_.clear();
  }
  this->last_frame_timestamp_ = now;

  if (this->frame_buffer_.size() > MAX_RESPONSE_SIZE) {
    ESP_LOGW(TAG, "Maximum response size exceeded");
    this->frame_buffer_.clear();
  }

  // Flush buffer on MODBUS frame start (device address + function code)
  if (length >= 2 && data[0] == LUMENTREE_MODBUS_DEVICE_ADDR &&
      (data[1] == LUMENTREE_MODBUS_FUNCTION_READ || data[1] == LUMENTREE_MODBUS_FUNCTION_READ_INPUT)) {
    if (!this->frame_buffer_.empty()) {
      ESP_LOGW(TAG, "New MODBUS frame detected, discarding incomplete frame (%d bytes)", this->frame_buffer_.size());
    }
    this->frame_buffer_.clear();
  }

  // Append received data to frame buffer
  this->frame_buffer_.insert(this->frame_buffer_.end(), data, data + length);

  // Check if we have enough data for a MODBUS response header
  if (this->frame_buffer_.size() < 5) {
    ESP_LOGVV(TAG, "Frame buffer too small (%d bytes), waiting for more data", this->frame_buffer_.size());
    return;
  }

  // Validate MODBUS response header
  if (this->frame_buffer_[0] != LUMENTREE_MODBUS_DEVICE_ADDR ||
      (this->frame_buffer_[1] != LUMENTREE_MODBUS_FUNCTION_READ &&
       this->frame_buffer_[1] != LUMENTREE_MODBUS_FUNCTION_READ_INPUT)) {
    ESP_LOGW(TAG, "Invalid MODBUS response header, clearing buffer");
    this->frame_buffer_.clear();
    return;
  }

  uint8_t byte_count = this->frame_buffer_[2];
  uint16_t expected_length = 3 + byte_count + 2;  // header + data + CRC

  if (this->frame_buffer_.size() < expected_length) {
    ESP_LOGVV(TAG, "Waiting for more data (%d/%d bytes)", this->frame_buffer_.size(), expected_length);
    return;
  }

  uint16_t calculated_crc = crc16(this->frame_buffer_.data(), 3 + byte_count, 0xFFFF, 0xa001, false, false);
  uint16_t received_crc = this->frame_buffer_[3 + byte_count] | (this->frame_buffer_[3 + byte_count + 1] << 8);

  if (calculated_crc != received_crc) {
    ESP_LOGW(TAG, "CRC check failed! 0x%04X != 0x%04X", calculated_crc, received_crc);
    this->frame_buffer_.clear();
    return;
  }

  ESP_LOGI(TAG, "Register data frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&this->frame_buffer_.front(), this->frame_buffer_.size()).c_str());

  this->decode_(this->frame_buffer_);
  this->frame_buffer_.clear();
}

void LumentreeBle::dump_config() {
  ESP_LOGCONFIG(TAG, "Lumentree BLE:");
  LOG_BINARY_SENSOR("  ", "Grid Connected", this->grid_connected_binary_sensor_);
  LOG_BINARY_SENSOR("  ", "Battery Connected", this->battery_connected_binary_sensor_);
  LOG_BINARY_SENSOR("  ", "PV2 Support", this->pv2_support_binary_sensor_);
  LOG_SENSOR("  ", "Battery Voltage", this->battery_voltage_sensor_);
  LOG_SENSOR("  ", "Battery Current", this->battery_current_sensor_);
  LOG_SENSOR("  ", "Battery Power", this->battery_power_sensor_);
  LOG_SENSOR("  ", "Battery SoC", this->battery_soc_sensor_);
  LOG_SENSOR("  ", "AC Output Voltage", this->ac_output_voltage_sensor_);
  LOG_SENSOR("  ", "AC Input Voltage", this->ac_input_voltage_sensor_);
  LOG_SENSOR("  ", "AC Output Frequency", this->ac_output_frequency_sensor_);
  LOG_SENSOR("  ", "AC Input Frequency", this->ac_input_frequency_sensor_);
  LOG_SENSOR("  ", "AC Power", this->ac_power_sensor_);
  LOG_SENSOR("  ", "PV Voltage", this->pv_voltage_sensor_);
  LOG_SENSOR("  ", "PV Power", this->pv_power_sensor_);
  LOG_SENSOR("  ", "Grid Power", this->grid_power_sensor_);
  LOG_SENSOR("  ", "Load Power", this->load_power_sensor_);
  LOG_SENSOR("  ", "Device Temperature", this->device_temperature_sensor_);
  LOG_SENSOR("  ", "PV2 Voltage", this->pv2_voltage_sensor_);
  LOG_SENSOR("  ", "PV2 Power", this->pv2_power_sensor_);
  LOG_SENSOR("  ", "Device Type Image", this->device_type_image_sensor_);
  LOG_SENSOR("  ", "Battery Status", this->battery_status_sensor_);
  LOG_SENSOR("  ", "Grid Connection Status", this->grid_connection_status_sensor_);
  LOG_SENSOR("  ", "Device Type", this->device_type_sensor_);
  LOG_SENSOR("  ", "Device Power Rating Code", this->device_power_rating_code_sensor_);
  LOG_SENSOR("  ", "Device Power Rating", this->device_power_rating_sensor_);
  LOG_SENSOR("  ", "AC Output Apparent Power", this->ac_output_apparent_power_sensor_);
  LOG_SENSOR("  ", "Grid CT Power", this->grid_ct_power_sensor_);
  LOG_TEXT_SENSOR("  ", "Serial Number", this->serial_number_text_sensor_);
  LOG_TEXT_SENSOR("  ", "Operation Mode", this->operation_mode_text_sensor_);
}

void LumentreeBle::update() {
  if (this->node_state != esp32_ble_tracker::ClientState::ESTABLISHED) {
    ESP_LOGW(TAG, "[%s] Not connected", this->parent_->address_str().c_str());
    return;
  }

  // Reset request sequence and start with system status
  this->current_request_type_ = REQUEST_SYSTEM_STATUS;
  ESP_LOGD(TAG, "[%s] Starting register data requests", this->parent_->address_str().c_str());
  this->send_next_request_();
}

void LumentreeBle::send_command(const std::vector<uint8_t> &payload) {
  std::vector<uint8_t> frame = payload;

  uint16_t crc = crc16(frame.data(), frame.size(), 0xFFFF, 0xa001, false, false);
  frame.push_back(crc & 0xFF);
  frame.push_back((crc >> 8) & 0xFF);

  ESP_LOGVV(TAG, "[%s] Command frame: %s", this->parent_->address_str().c_str(), format_hex_pretty(frame).c_str());

  auto status = esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(),
                                         this->char_handle_, frame.size(), const_cast<uint8_t *>(frame.data()),
                                         ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);
  if (status) {
    ESP_LOGW(TAG, "[%s] Error sending command, status=%d", this->parent_->address_str().c_str(), status);
  }
}

void LumentreeBle::read_registers(uint16_t start_register, uint16_t register_count) {
  std::vector<uint8_t> payload;
  payload.push_back(LUMENTREE_MODBUS_DEVICE_ADDR);
  payload.push_back(LUMENTREE_MODBUS_FUNCTION_READ);
  payload.push_back((start_register >> 8) & 0xFF);
  payload.push_back(start_register & 0xFF);
  payload.push_back((register_count >> 8) & 0xFF);
  payload.push_back(register_count & 0xFF);

  ESP_LOGD(TAG, "[%s] Reading Holding Registers: start=0x%04X, count=%d", this->parent_->address_str().c_str(),
           start_register, register_count);

  this->send_command(payload);
}

void LumentreeBle::decode_(const std::vector<uint8_t> &data) {
  if (data.size() < 3) {
    ESP_LOGW(TAG, "Response data too short");
    return;
  }

  uint8_t byte_count = data[2];
  if (data.size() < 3 + byte_count) {
    ESP_LOGW(TAG, "Incomplete register data");
    return;
  }

  // Validate response format based on function code
  uint8_t function_code = data[1];
  if (function_code == LUMENTREE_MODBUS_FUNCTION_READ) {
    ESP_LOGD(TAG, "Received Read Holding Registers response (0x03)");
  } else if (function_code == LUMENTREE_MODBUS_FUNCTION_READ_INPUT) {
    ESP_LOGD(TAG, "Received Read Input Registers response (0x04)");
  } else {
    ESP_LOGW(TAG, "Unexpected function code in response: 0x%02X", function_code);
    return;
  }

  ESP_LOGI(TAG, "Decoding %d register bytes for request type %d (function 0x%02X)", byte_count,
           this->current_request_type_, function_code);

  // Delegate to specific decoder based on current request type
  switch (this->current_request_type_) {
    case REQUEST_SYSTEM_STATUS:
      this->decode_system_status_registers_(data);
      break;
    case REQUEST_BATTERY_CONFIG:
      this->decode_battery_config_registers_(data);
      break;
    case REQUEST_SYSTEM_CONTROL:
      this->decode_system_control_registers_(data);
      break;
    default:
      ESP_LOGW(TAG, "Unknown request type: %d", this->current_request_type_);
      break;
  }

  // Move to next request
  this->current_request_type_ = static_cast<RequestType>(this->current_request_type_ + 1);

  // Send next request after a short delay
  if (this->current_request_type_ < REQUEST_COMPLETE) {
    this->set_timeout("next_request", 100, [this]() { this->send_next_request_(); });
  }
}

void LumentreeBle::decode_system_status_registers_(const std::vector<uint8_t> &data) {
  auto lumentree_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };

  uint8_t byte_count = data[2];
  ESP_LOGI(TAG, "Decoding System Status registers (0-94)");

  // 0x03-0x07: Serial Number (5 registers, ASCII)
  std::string serial_number(data.begin() + 9, data.begin() + 19);
  serial_number.erase(serial_number.find_last_not_of(" \0") + 1);
  ESP_LOGI(TAG, "Serial Number: %s", serial_number.c_str());
  this->publish_state_(this->serial_number_text_sensor_, serial_number.empty() ? "Unknown" : serial_number);

  // Register offsets in the data array (3 bytes header + register_index * 2)
  for (uint8_t register_index = 0; register_index < byte_count / 2; register_index++) {
    uint16_t register_value = lumentree_get_16bit(3 + register_index * 2);

    switch (register_index) {
      case 0:  // 0x00: Device Type
        this->publish_state_(this->device_type_sensor_, register_value);
        break;
      case 1:  // 0x01: Device Configuration (PV2 Support)
        this->publish_state_(this->pv2_support_binary_sensor_, register_value == 0x0102);
        break;
      case 2:  // 0x02: Device Type Code
        ESP_LOGVV(TAG, "Device Type Code: 0x%04X", register_value);
        break;
      case 8:  // 0x08: Power Level (2=5.5KW, 3=4.0KW, 5=6.0KW, other=3.6KW)
        this->publish_state_(this->device_power_rating_code_sensor_, register_value);
        this->publish_state_(this->device_power_rating_sensor_, this->power_rating_code_to_watts_(register_value));
        break;
      case 11:  // 0x0B: Battery Voltage
        this->publish_state_(this->battery_voltage_sensor_, register_value * 0.01f);
        break;
      case 12:  // 0x0C: Battery Current
        this->publish_state_(this->battery_current_sensor_, (int16_t) register_value * 0.01f);
        break;
      case 13:  // 0x0D: AC Output Voltage
        this->publish_state_(this->ac_output_voltage_sensor_, register_value * 0.1f);
        break;
      case 15:  // 0x0F: AC Input Voltage
        this->publish_state_(this->ac_input_voltage_sensor_, register_value * 0.1f);
        break;
      case 16:  // 0x10: AC Output Frequency
        this->publish_state_(this->ac_output_frequency_sensor_, register_value * 0.01f);
        break;
      case 17:  // 0x11: AC Input Frequency
        this->publish_state_(this->ac_input_frequency_sensor_, register_value * 0.01f);
        break;
      case 18:  // 0x12: AC Output Power
        this->publish_state_(this->ac_power_sensor_, register_value * 1.0f);
        break;
      case 20:  // 0x14: PV Input Voltage
        this->publish_state_(this->pv_voltage_sensor_, register_value * 1.0f);
        break;
      case 22:  // 0x16: PV Input Power
        this->publish_state_(this->pv_power_sensor_, register_value * 1.0f);
        break;
      case 24:  // 0x18: IGBT Temperature
        this->publish_state_(this->device_temperature_sensor_, (register_value - 1000) * 0.1f);
        break;
      case 37:  // 0x25: Battery Status
        this->publish_state_(this->battery_connected_binary_sensor_, register_value != 2);
        this->publish_state_(this->battery_status_sensor_, register_value * 1.0f);
        break;
      case 50:  // 0x32: Battery State of Charge
        this->publish_state_(this->battery_soc_sensor_, register_value * 1.0f);
        break;
      case 53:  // 0x35: Grid Input Power (signed)
        this->publish_state_(this->grid_power_sensor_, (int16_t) register_value * 1.0f);
        break;
      case 58:  // 0x3A: AC Output VA/Apparent Power
        this->publish_state_(this->ac_output_apparent_power_sensor_, register_value * 1.0f);
        break;
      case 59:  // 0x3B: Grid CT Power
        this->publish_state_(this->grid_ct_power_sensor_, register_value * 1.0f);
        break;
      case 61:  // 0x3D: Battery Power (signed)
        this->publish_state_(this->battery_power_sensor_, (int16_t) register_value * 1.0f);
        break;
      case 67:  // 0x43: Load Power
        this->publish_state_(this->load_power_sensor_, register_value * 1.0f);
        break;
      case 68:  // 0x44: Operation Mode
        this->publish_state_(this->operation_mode_text_sensor_, this->operation_mode_to_string_(register_value));
        break;
      case 70:  // 0x46: Grid Connection Status
        this->publish_state_(this->grid_connected_binary_sensor_, register_value >= 7);
        this->publish_state_(this->grid_connection_status_sensor_, register_value * 1.0f);
        break;
      case 72:  // 0x48: PV Input Voltage 2
        this->publish_state_(this->pv2_voltage_sensor_, register_value * 1.0f);
        break;
      case 74:  // 0x4A: PV Input Power 2
        this->publish_state_(this->pv2_power_sensor_, register_value * 1.0f);
        break;
      case 94:  // 0x5E: Device Type Image
        this->publish_state_(this->device_type_image_sensor_, register_value * 1.0f);
        break;
      default:
        ESP_LOGVV(TAG, "Register %d: 0x%04X (%d)", register_index, register_value, register_value);
        break;
    }
  }
}

void LumentreeBle::decode_battery_config_registers_(const std::vector<uint8_t> &data) {
  auto lumentree_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };

  uint8_t byte_count = data[2];
  ESP_LOGI(TAG, "Decoding Battery Configuration registers (101-157)");

  // Register offsets in the data array (3 bytes header + register_index * 2)
  // Note: For registers 101-157, we need to offset by 101 to get the correct register address
  for (uint8_t i = 0; i < byte_count / 2; i++) {
    uint16_t register_value = lumentree_get_16bit(3 + i * 2);
    uint8_t register_address = 101 + i;  // Actual register address

    switch (register_address) {
      case 101:  // Equalization Voltage
        ESP_LOGI(TAG, "Equalization Voltage: %.2f V", register_value * 0.01f);
        break;
      case 102:  // Charging Target Voltage
        ESP_LOGI(TAG, "Charging Target Voltage: %.2f V", register_value * 0.01f);
        break;
      case 103:  // Float Charge Voltage
        ESP_LOGI(TAG, "Float Charge Voltage: %.2f V", register_value * 0.01f);
        break;
      case 104:  // Battery Capacity
        ESP_LOGI(TAG, "Battery Capacity: %d Ah", register_value);
        break;
      case 114:  // Low Voltage Cutoff
        ESP_LOGI(TAG, "Low Voltage Cutoff: %.2f V", register_value * 0.01f);
        break;
      case 115:  // Battery Voltage Restore
        ESP_LOGI(TAG, "Battery Voltage Restore: %.2f V", register_value * 0.01f);
        break;
      case 117:  // Target Voltage 5
        ESP_LOGI(TAG, "Target Voltage 5: %.2f V", register_value * 0.01f);
        break;
      case 118:  // Target Voltage 6
        ESP_LOGI(TAG, "Target Voltage 6: %.2f V", register_value * 0.01f);
        break;
      case 120:  // Charge from AC
        ESP_LOGI(TAG, "Charge from AC: %s", register_value == 1 ? "Enabled" : "Disabled");
        break;
      case 123:  // AC Output Frequency
        ESP_LOGI(TAG, "AC Output Frequency: %s", register_value == 0 ? "50Hz" : "60Hz");
        break;
      case 125:  // AC Output Voltage
        ESP_LOGI(TAG, "AC Output Voltage Setting: %d V", register_value);
        break;
      case 143:  // Maximum Current Setting 1
        ESP_LOGI(TAG, "Maximum Current Setting 1: %d A", register_value);
        break;
      case 144:  // Maximum Current Setting 2
        ESP_LOGI(TAG, "Maximum Current Setting 2: %d A", register_value);
        break;
      case 145:  // Maximum Current Setting 3
        ESP_LOGI(TAG, "Maximum Current Setting 3: %d A", register_value);
        break;
      case 146:  // Maximum Current Setting 4
        ESP_LOGI(TAG, "Maximum Current Setting 4: %d A", register_value);
        break;
      case 148:  // Equalized Charge Interval
        ESP_LOGI(TAG, "Equalized Charge Interval: %d days", register_value);
        break;
      case 149:  // Balanced Charging Duration
        ESP_LOGI(TAG, "Balanced Charging Duration: %d minutes", register_value);
        break;
      case 150:  // Work Mode
        ESP_LOGI(TAG, "Work Mode: %d", register_value);
        break;
      case 152:  // Starter Generator
        ESP_LOGI(TAG, "Starter Generator: %s", register_value == 1 ? "Enabled" : "Disabled");
        break;
      case 153:  // Target Voltage 1
        ESP_LOGI(TAG, "Target Voltage 1: %.2f V", register_value * 0.01f);
        break;
      case 154:  // Target Voltage 2
        ESP_LOGI(TAG, "Target Voltage 2: %.2f V", register_value * 0.01f);
        break;
      case 155:  // Target Voltage 3
        ESP_LOGI(TAG, "Target Voltage 3: %.2f V", register_value * 0.01f);
        break;
      case 156:  // Target Voltage 4
        ESP_LOGI(TAG, "Target Voltage 4: %.2f V", register_value * 0.01f);
        break;
      case 157:  // Maximum Discharge Current
        ESP_LOGI(TAG, "Maximum Discharge Current: %d A", register_value);
        break;
      default:
        ESP_LOGVV(TAG, "Battery Configuration Register %d: 0x%04X (%d)", register_address, register_value,
                  register_value);
        break;
    }
  }
}

void LumentreeBle::decode_system_control_registers_(const std::vector<uint8_t> &data) {
  auto lumentree_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };

  uint8_t byte_count = data[2];
  ESP_LOGI(TAG, "Decoding System Control registers (160-180)");

  // Register offsets in the data array (3 bytes header + register_index * 2)
  // Note: For registers 160-180, we need to offset by 160 to get the correct register address
  for (uint8_t i = 0; i < byte_count / 2; i++) {
    uint16_t register_value = lumentree_get_16bit(3 + i * 2);
    uint8_t register_address = 160 + i;  // Actual register address

    switch (register_address) {
      case 160:  // Real Time Clock
        ESP_LOGI(TAG, "Real Time Clock: %d (Unix timestamp)", register_value);
        break;
      case 163:  // Sleep Mode Set
        ESP_LOGI(TAG, "Sleep Mode: %s", register_value == 1 ? "Enabled" : "Disabled");
        break;
      case 165:  // Overload Auto Start
        ESP_LOGI(TAG, "Overload Auto Start: %s", register_value == 1 ? "Enabled" : "Disabled");
        break;
      case 166:  // Overtemperature Protection Auto
        ESP_LOGI(TAG, "Overtemperature Protection Auto: %s", register_value == 1 ? "Enabled" : "Disabled");
        break;
      case 167:  // Beep
        ESP_LOGI(TAG, "Beep: %s", register_value == 1 ? "Enabled" : "Silent");
        break;
      case 168:  // Backlight
        ESP_LOGI(TAG, "Backlight: %d", register_value);
        break;
      case 170:  // Online Mode
        ESP_LOGI(TAG, "Online Mode: %s", register_value == 1 ? "Online" : "Offline");
        break;
      case 177:  // Maximum Current 5
        ESP_LOGI(TAG, "Maximum Current 5: %d A", register_value);
        break;
      case 178:  // Maximum Current 6
        ESP_LOGI(TAG, "Maximum Current 6: %d A", register_value);
        break;
      default:
        ESP_LOGVV(TAG, "System Control Register %d: 0x%04X (%d)", register_address, register_value, register_value);
        break;
    }
  }
}

void LumentreeBle::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void LumentreeBle::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void LumentreeBle::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

std::string LumentreeBle::operation_mode_to_string_(uint16_t mode) {
  switch (mode) {
    case 0:
      return "Battery Mode";
    case 1:
      return "Hybrid Mode";
    case 2:
      return "Grid-Tie Mode";
    default:
      return str_snprintf("Unknown (0x%04X)", 17, mode);
  }
}

void LumentreeBle::send_next_request_() {
  switch (this->current_request_type_) {
    case REQUEST_SYSTEM_STATUS:
      ESP_LOGI(TAG, "Requesting System Status registers (0-94)");
      this->read_registers(0, 95);
      break;
    case REQUEST_BATTERY_CONFIG:
      ESP_LOGI(TAG, "Requesting Battery Configuration registers (101-157)");
      this->read_registers(101, 57);
      break;
    case REQUEST_SYSTEM_CONTROL:
      ESP_LOGI(TAG, "Requesting System Control registers (160-180)");
      this->read_registers(160, 21);
      break;
    case REQUEST_COMPLETE:
      ESP_LOGI(TAG, "All register requests completed");
      break;
  }
}

void LumentreeBle::write_register(uint8_t register_address, uint16_t value) {
  ESP_LOGI(TAG, "Writing register 0x%02X with value 0x%04X", register_address, value);

  std::vector<uint8_t> payload;
  payload.push_back(LUMENTREE_MODBUS_DEVICE_ADDR);
  payload.push_back(0x06);
  payload.push_back(0x00);
  payload.push_back(register_address);
  payload.push_back((value >> 8) & 0xFF);
  payload.push_back(value & 0xFF);

  this->send_command(payload);
}

float LumentreeBle::power_rating_code_to_watts_(uint16_t code) {
  switch (code) {
    case 2:
      return 5500.0f;  // 5.5KW
    case 3:
      return 4000.0f;  // 4.0KW
    case 5:
      return 6000.0f;  // 6.0KW
    default:
      return 3600.0f;  // 3.6KW (default)
  }
}

}  // namespace lumentree_ble
}  // namespace esphome

#endif
