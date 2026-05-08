#include "esphome/components/lumentree_ble/lumentree_ble.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "common.h"
#include "frames.h"
#include <gtest/gtest.h>

namespace esphome::lumentree_ble::testing {

class LumentreeSystemStatusTest : public ::testing::Test {
 protected:
  TestableLumentreeBle bms_;
  sensor::Sensor battery_voltage_;
  sensor::Sensor battery_current_;
  sensor::Sensor ac_output_voltage_;
  sensor::Sensor ac_input_voltage_;
  sensor::Sensor ac_output_frequency_;
  sensor::Sensor ac_input_frequency_;
  sensor::Sensor ac_power_;
  sensor::Sensor pv_voltage_;
  sensor::Sensor pv_power_;
  sensor::Sensor device_temperature_;
  sensor::Sensor device_type_;
  sensor::Sensor device_power_rating_code_;
  sensor::Sensor device_power_rating_;
  text_sensor::TextSensor serial_number_;
  text_sensor::TextSensor device_model_;

  void SetUp() override {
    bms_.set_battery_voltage_sensor(&battery_voltage_);
    bms_.set_battery_current_sensor(&battery_current_);
    bms_.set_ac_output_voltage_sensor(&ac_output_voltage_);
    bms_.set_ac_input_voltage_sensor(&ac_input_voltage_);
    bms_.set_ac_output_frequency_sensor(&ac_output_frequency_);
    bms_.set_ac_input_frequency_sensor(&ac_input_frequency_);
    bms_.set_ac_power_sensor(&ac_power_);
    bms_.set_pv_voltage_sensor(&pv_voltage_);
    bms_.set_pv_power_sensor(&pv_power_);
    bms_.set_device_temperature_sensor(&device_temperature_);
    bms_.set_device_type_sensor(&device_type_);
    bms_.set_device_power_rating_code_sensor(&device_power_rating_code_);
    bms_.set_device_power_rating_sensor(&device_power_rating_);
    bms_.set_serial_number_text_sensor(&serial_number_);
    bms_.set_device_model_text_sensor(&device_model_);
    bms_.set_last_request(LumentreeBle::REQUEST_SYSTEM_STATUS);
    bms_.assemble(SYSTEM_STATUS_FRAME.data(), SYSTEM_STATUS_FRAME.size());
  }
};

TEST_F(LumentreeSystemStatusTest, BatteryMeasurements) {
  EXPECT_NEAR(battery_voltage_.state, 52.90f, 0.01f);
  EXPECT_NEAR(battery_current_.state, 8.30f, 0.01f);
}

TEST_F(LumentreeSystemStatusTest, AcMeasurements) {
  EXPECT_NEAR(ac_output_voltage_.state, 237.0f, 0.1f);
  EXPECT_NEAR(ac_input_voltage_.state, 235.0f, 0.1f);
  EXPECT_NEAR(ac_output_frequency_.state, 49.70f, 0.01f);
  EXPECT_NEAR(ac_input_frequency_.state, 49.70f, 0.01f);
  EXPECT_NEAR(ac_power_.state, 500.0f, 0.1f);
}

TEST_F(LumentreeSystemStatusTest, PvMeasurements) {
  EXPECT_NEAR(pv_voltage_.state, 177.0f, 0.1f);
  EXPECT_NEAR(pv_power_.state, 23.0f, 0.1f);
}

TEST_F(LumentreeSystemStatusTest, Temperature) { EXPECT_NEAR(device_temperature_.state, 33.6f, 0.1f); }

TEST_F(LumentreeSystemStatusTest, DeviceInfo) {
  EXPECT_NEAR(device_type_.state, 768.0f, 0.1f);
  EXPECT_NEAR(device_power_rating_code_.state, 5.0f, 0.1f);
  EXPECT_NEAR(device_power_rating_.state, 6000.0f, 0.1f);
}

TEST_F(LumentreeSystemStatusTest, SerialNumber) { EXPECT_EQ(serial_number_.state, "P241019070"); }

TEST_F(LumentreeSystemStatusTest, DeviceModel) { EXPECT_EQ(device_model_.state, "SUNT-6.0KW-P"); }

TEST(LumentreeSafetyTest, NullSensorsDoNotCrash) {
  TestableLumentreeBle bms;
  bms.set_last_request(LumentreeBle::REQUEST_SYSTEM_STATUS);
  bms.assemble(SYSTEM_STATUS_FRAME.data(), SYSTEM_STATUS_FRAME.size());
}

}  // namespace esphome::lumentree_ble::testing
