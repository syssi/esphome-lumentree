#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace lumentree_ble {

namespace espbt = esphome::esp32_ble_tracker;

class LumentreeBle : public esphome::ble_client::BLEClientNode, public PollingComponent {
 public:
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) {
    battery_voltage_sensor_ = battery_voltage_sensor;
  }
  void set_battery_current_sensor(sensor::Sensor *battery_current_sensor) {
    battery_current_sensor_ = battery_current_sensor;
  }
  void set_battery_power_sensor(sensor::Sensor *battery_power_sensor) { battery_power_sensor_ = battery_power_sensor; }
  void set_battery_soc_sensor(sensor::Sensor *battery_soc_sensor) { battery_soc_sensor_ = battery_soc_sensor; }
  void set_ac_voltage_sensor(sensor::Sensor *ac_voltage_sensor) { ac_voltage_sensor_ = ac_voltage_sensor; }
  void set_ac_power_sensor(sensor::Sensor *ac_power_sensor) { ac_power_sensor_ = ac_power_sensor; }
  void set_pv_voltage_sensor(sensor::Sensor *pv_voltage_sensor) { pv_voltage_sensor_ = pv_voltage_sensor; }
  void set_pv_power_sensor(sensor::Sensor *pv_power_sensor) { pv_power_sensor_ = pv_power_sensor; }
  void set_grid_power_sensor(sensor::Sensor *grid_power_sensor) { grid_power_sensor_ = grid_power_sensor; }
  void set_load_power_sensor(sensor::Sensor *load_power_sensor) { load_power_sensor_ = load_power_sensor; }
  void set_device_temperature_sensor(sensor::Sensor *device_temperature_sensor) {
    device_temperature_sensor_ = device_temperature_sensor;
  }
  void set_pv2_voltage_sensor(sensor::Sensor *pv2_voltage_sensor) { pv2_voltage_sensor_ = pv2_voltage_sensor; }
  void set_pv2_power_sensor(sensor::Sensor *pv2_power_sensor) { pv2_power_sensor_ = pv2_power_sensor; }
  void set_device_type_image_sensor(sensor::Sensor *device_type_image_sensor) {
    device_type_image_sensor_ = device_type_image_sensor;
  }
  void set_battery_status_sensor(sensor::Sensor *battery_status_sensor) {
    battery_status_sensor_ = battery_status_sensor;
  }
  void set_grid_connection_status_sensor(sensor::Sensor *grid_connection_status_sensor) {
    grid_connection_status_sensor_ = grid_connection_status_sensor;
  }

  void set_grid_connected_binary_sensor(binary_sensor::BinarySensor *grid_connected_binary_sensor) {
    grid_connected_binary_sensor_ = grid_connected_binary_sensor;
  }
  void set_battery_connected_binary_sensor(binary_sensor::BinarySensor *battery_connected_binary_sensor) {
    battery_connected_binary_sensor_ = battery_connected_binary_sensor;
  }

  void set_device_model_text_sensor(text_sensor::TextSensor *device_model_text_sensor) {
    device_model_text_sensor_ = device_model_text_sensor;
  }
  void set_operation_mode_text_sensor(text_sensor::TextSensor *operation_mode_text_sensor) {
    operation_mode_text_sensor_ = operation_mode_text_sensor;
  }

  void assemble(const uint8_t *data, uint16_t length);

 protected:
  binary_sensor::BinarySensor *grid_connected_binary_sensor_;
  binary_sensor::BinarySensor *battery_connected_binary_sensor_;

  sensor::Sensor *battery_voltage_sensor_;
  sensor::Sensor *battery_current_sensor_;
  sensor::Sensor *battery_power_sensor_;
  sensor::Sensor *battery_soc_sensor_;
  sensor::Sensor *ac_voltage_sensor_;
  sensor::Sensor *ac_power_sensor_;
  sensor::Sensor *pv_voltage_sensor_;
  sensor::Sensor *pv_power_sensor_;
  sensor::Sensor *grid_power_sensor_;
  sensor::Sensor *load_power_sensor_;
  sensor::Sensor *device_temperature_sensor_;
  sensor::Sensor *pv2_voltage_sensor_;
  sensor::Sensor *pv2_power_sensor_;
  sensor::Sensor *device_type_image_sensor_;
  sensor::Sensor *battery_status_sensor_;
  sensor::Sensor *grid_connection_status_sensor_;

  text_sensor::TextSensor *device_model_text_sensor_;
  text_sensor::TextSensor *operation_mode_text_sensor_;

  uint16_t char_handle_;
  std::vector<uint8_t> frame_buffer_;
  uint32_t last_frame_timestamp_ = 0;

  // Request tracking
  enum RequestType { REQUEST_SYSTEM_STATUS, REQUEST_BATTERY_CONFIG, REQUEST_SYSTEM_CONTROL, REQUEST_COMPLETE };
  RequestType current_request_type_ = REQUEST_SYSTEM_STATUS;

  void decode_(const std::vector<uint8_t> &data);
  void decode_system_status_registers_(const std::vector<uint8_t> &data);
  void decode_battery_config_registers_(const std::vector<uint8_t> &data);
  void decode_system_control_registers_(const std::vector<uint8_t> &data);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
  void send_next_request_();
  void send_command_(uint8_t function_code, uint16_t start_register, uint16_t register_count);
  std::string operation_mode_to_string_(uint16_t mode);
};

}  // namespace lumentree_ble
}  // namespace esphome

#endif
