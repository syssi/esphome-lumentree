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
#include "esphome/components/select/select.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace lumentree_ble {

namespace espbt = esphome::esp32_ble_tracker;

class LumentreeBle : public esphome::ble_client::BLEClientNode, public PollingComponent {
 public:
  enum RequestType {
    REQUEST_SYSTEM_STATUS,
    REQUEST_BATTERY_CONFIG,
    REQUEST_SYSTEM_CONTROL,
    REQUEST_DAILY_STATISTICS,
    REQUEST_COMPLETE
  };

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
  void set_ac_output_voltage_sensor(sensor::Sensor *ac_output_voltage_sensor) {
    ac_output_voltage_sensor_ = ac_output_voltage_sensor;
  }
  void set_ac_input_voltage_sensor(sensor::Sensor *ac_input_voltage_sensor) {
    ac_input_voltage_sensor_ = ac_input_voltage_sensor;
  }
  void set_ac_output_frequency_sensor(sensor::Sensor *ac_output_frequency_sensor) {
    ac_output_frequency_sensor_ = ac_output_frequency_sensor;
  }
  void set_ac_input_frequency_sensor(sensor::Sensor *ac_input_frequency_sensor) {
    ac_input_frequency_sensor_ = ac_input_frequency_sensor;
  }
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
  void set_device_type_sensor(sensor::Sensor *device_type_sensor) { device_type_sensor_ = device_type_sensor; }
  void set_device_power_rating_code_sensor(sensor::Sensor *device_power_rating_code_sensor) {
    device_power_rating_code_sensor_ = device_power_rating_code_sensor;
  }
  void set_device_power_rating_sensor(sensor::Sensor *device_power_rating_sensor) {
    device_power_rating_sensor_ = device_power_rating_sensor;
  }
  void set_ac_output_apparent_power_sensor(sensor::Sensor *ac_output_apparent_power_sensor) {
    ac_output_apparent_power_sensor_ = ac_output_apparent_power_sensor;
  }
  void set_grid_ct_power_sensor(sensor::Sensor *grid_ct_power_sensor) { grid_ct_power_sensor_ = grid_ct_power_sensor; }
  void set_today_pv_production_sensor(sensor::Sensor *today_pv_production_sensor) {
    today_pv_production_sensor_ = today_pv_production_sensor;
  }
  void set_today_essential_load_sensor(sensor::Sensor *today_essential_load_sensor) {
    today_essential_load_sensor_ = today_essential_load_sensor;
  }
  void set_today_total_consumption_sensor(sensor::Sensor *today_total_consumption_sensor) {
    today_total_consumption_sensor_ = today_total_consumption_sensor;
  }
  void set_today_grid_consumption_sensor(sensor::Sensor *today_grid_consumption_sensor) {
    today_grid_consumption_sensor_ = today_grid_consumption_sensor;
  }
  void set_today_battery_charging_sensor(sensor::Sensor *today_battery_charging_sensor) {
    today_battery_charging_sensor_ = today_battery_charging_sensor;
  }
  void set_today_battery_discharge_sensor(sensor::Sensor *today_battery_discharge_sensor) {
    today_battery_discharge_sensor_ = today_battery_discharge_sensor;
  }
  void set_grid_export_sensor(sensor::Sensor *grid_export_sensor) { grid_export_sensor_ = grid_export_sensor; }

  void set_grid_connected_binary_sensor(binary_sensor::BinarySensor *grid_connected_binary_sensor) {
    grid_connected_binary_sensor_ = grid_connected_binary_sensor;
  }
  void set_battery_connected_binary_sensor(binary_sensor::BinarySensor *battery_connected_binary_sensor) {
    battery_connected_binary_sensor_ = battery_connected_binary_sensor;
  }
  void set_pv2_support_binary_sensor(binary_sensor::BinarySensor *pv2_support_binary_sensor) {
    pv2_support_binary_sensor_ = pv2_support_binary_sensor;
  }

  void set_serial_number_text_sensor(text_sensor::TextSensor *serial_number_text_sensor) {
    serial_number_text_sensor_ = serial_number_text_sensor;
  }
  void set_operation_mode_text_sensor(text_sensor::TextSensor *operation_mode_text_sensor) {
    operation_mode_text_sensor_ = operation_mode_text_sensor;
  }
  void set_device_model_text_sensor(text_sensor::TextSensor *device_model_text_sensor) {
    device_model_text_sensor_ = device_model_text_sensor;
  }

  void set_factory_reset_button(button::Button *factory_reset_button) { factory_reset_button_ = factory_reset_button; }
  void set_restart_device_button(button::Button *restart_device_button) {
    restart_device_button_ = restart_device_button;
  }

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

  void set_ac_charging_switch(switch_::Switch *ac_charging_switch) { ac_charging_switch_ = ac_charging_switch; }
  void set_output_switch(switch_::Switch *output_switch) { output_switch_ = output_switch; }

  void set_operation_mode_select(select::Select *operation_mode_select) {
    operation_mode_select_ = operation_mode_select;
  }

  void assemble(const uint8_t *data, uint16_t length);
  void set_last_request(RequestType request_type);
  void write_register(uint8_t register_address, uint16_t value);
  void write_multiple_registers(uint8_t start_register, const std::vector<uint16_t> &values);
  void send_command(const std::vector<uint8_t> &payload);
  void read_registers(uint8_t function, uint16_t start_register, uint16_t register_count);

 protected:
  binary_sensor::BinarySensor *grid_connected_binary_sensor_;
  binary_sensor::BinarySensor *battery_connected_binary_sensor_;
  binary_sensor::BinarySensor *pv2_support_binary_sensor_;

  sensor::Sensor *battery_voltage_sensor_;
  sensor::Sensor *battery_current_sensor_;
  sensor::Sensor *battery_power_sensor_;
  sensor::Sensor *battery_soc_sensor_;
  sensor::Sensor *ac_output_voltage_sensor_;
  sensor::Sensor *ac_input_voltage_sensor_;
  sensor::Sensor *ac_output_frequency_sensor_;
  sensor::Sensor *ac_input_frequency_sensor_;
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
  sensor::Sensor *device_type_sensor_;
  sensor::Sensor *device_power_rating_code_sensor_;
  sensor::Sensor *device_power_rating_sensor_;
  sensor::Sensor *ac_output_apparent_power_sensor_;
  sensor::Sensor *grid_ct_power_sensor_;
  sensor::Sensor *today_pv_production_sensor_;
  sensor::Sensor *today_essential_load_sensor_;
  sensor::Sensor *today_total_consumption_sensor_;
  sensor::Sensor *today_grid_consumption_sensor_;
  sensor::Sensor *today_battery_charging_sensor_;
  sensor::Sensor *today_battery_discharge_sensor_;
  sensor::Sensor *grid_export_sensor_;

  text_sensor::TextSensor *serial_number_text_sensor_;
  text_sensor::TextSensor *operation_mode_text_sensor_;
  text_sensor::TextSensor *device_model_text_sensor_;

  button::Button *factory_reset_button_;
  button::Button *restart_device_button_;

  number::Number *power_output_setting_number_;
  number::Number *equalization_voltage_setting_number_;
  number::Number *charging_target_voltage_setting_number_;
  number::Number *float_charge_voltage_setting_number_;
  number::Number *battery_capacity_setting_number_;

  switch_::Switch *ac_charging_switch_;
  switch_::Switch *output_switch_;

  select::Select *operation_mode_select_;

  uint16_t char_handle_;
  std::vector<uint8_t> frame_buffer_;
  uint32_t last_frame_timestamp_ = 0;

  RequestType current_request_type_ = REQUEST_SYSTEM_STATUS;

  void decode_(const std::vector<uint8_t> &data);
  void decode_system_status_registers_(const std::vector<uint8_t> &data);
  void decode_battery_config_registers_(const std::vector<uint8_t> &data);
  void decode_system_control_registers_(const std::vector<uint8_t> &data);
  void decode_daily_statistics_registers_(const std::vector<uint8_t> &data);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
  void publish_state_(switch_::Switch *switch_entity, const bool &state);
  void publish_state_(select::Select *select_entity, const std::string &state);
  void send_next_request_();
  std::string operation_mode_to_string_(uint16_t mode);
  float power_rating_code_to_watts_(uint16_t code);
  std::string generate_device_model_(uint16_t device_type, uint16_t power_rating, bool light_engine);
};

}  // namespace lumentree_ble
}  // namespace esphome

#endif
