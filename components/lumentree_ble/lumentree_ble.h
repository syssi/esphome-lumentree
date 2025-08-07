#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/button/button.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"

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

  // Button setters
  void set_factory_reset_button(button::Button *factory_reset_button) { factory_reset_button_ = factory_reset_button; }
  void set_restart_device_button(button::Button *restart_device_button) {
    restart_device_button_ = restart_device_button;
  }

  // Number setters
  void set_power_output_setting_number(number::Number *power_output_setting_number) {
    power_output_setting_number_ = power_output_setting_number;
  }
  void set_equalization_voltage_setting_number(number::Number *equalization_voltage_setting_number) {
    equalization_voltage_setting_number_ = equalization_voltage_setting_number;
  }
  void set_charging_target_voltage_setting_number(number::Number *charging_target_voltage_setting_number) {
    charging_target_voltage_setting_number_ = charging_target_voltage_setting_number;
  }
  void set_float_charge_voltage_setting_number(number::Number *float_charge_voltage_setting_number) {
    float_charge_voltage_setting_number_ = float_charge_voltage_setting_number;
  }
  void set_battery_capacity_setting_number(number::Number *battery_capacity_setting_number) {
    battery_capacity_setting_number_ = battery_capacity_setting_number;
  }

  // Switch setters
  void set_ac_charging_switch(switch_::Switch *ac_charging_switch) { ac_charging_switch_ = ac_charging_switch; }
  void set_output_enable_switch(switch_::Switch *output_enable_switch) { output_enable_switch_ = output_enable_switch; }

  void assemble(const uint8_t *data, uint16_t length);
  void write_register(uint8_t register_address, uint16_t value);
  void send_command(const std::vector<uint8_t> &frame);
  void read_registers(uint16_t start_register, uint16_t register_count);

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

  // Button members
  button::Button *factory_reset_button_;
  button::Button *restart_device_button_;

  // Number members
  number::Number *power_output_setting_number_;
  number::Number *equalization_voltage_setting_number_;
  number::Number *charging_target_voltage_setting_number_;
  number::Number *float_charge_voltage_setting_number_;
  number::Number *battery_capacity_setting_number_;

  // Switch members
  switch_::Switch *ac_charging_switch_;
  switch_::Switch *output_enable_switch_;

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
  std::string operation_mode_to_string_(uint16_t mode);
};

}  // namespace lumentree_ble
}  // namespace esphome

#endif
