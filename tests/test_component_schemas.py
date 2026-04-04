"""Schema structure tests for lumentree_ble ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.lumentree_ble as hub  # noqa: E402
from components.lumentree_ble import (  # noqa: E402
    binary_sensor,
    button,  # noqa: E402
    number,  # noqa: E402
    select as select_mod,  # noqa: E402
    sensor,
    switch,  # noqa: E402
    text_sensor,
)


class TestHubConstants:
    def test_conf_id_defined(self):
        assert hub.CONF_LUMENTREE_BLE_ID == "lumentree_ble_id"


class TestSensorLists:
    def test_sensors_completeness(self):
        assert "battery_voltage" in sensor.SENSORS
        assert "battery_current" in sensor.SENSORS
        assert "battery_power" in sensor.SENSORS
        assert "battery_soc" in sensor.SENSORS
        assert "pv_voltage" in sensor.SENSORS
        assert "pv_power" in sensor.SENSORS
        assert "grid_power" in sensor.SENSORS
        assert "load_power" in sensor.SENSORS
        assert "device_temperature" in sensor.SENSORS
        assert "today_pv_production" in sensor.SENSORS
        assert len(sensor.SENSORS) == 31

    def test_sensor_keys_are_strings(self):
        for key in sensor.SENSORS:
            assert isinstance(key, str)


class TestBinarySensorConstants:
    def test_binary_sensors_dict(self):
        assert binary_sensor.CONF_GRID_CONNECTED in binary_sensor.BINARY_SENSORS
        assert binary_sensor.CONF_BATTERY_CONNECTED in binary_sensor.BINARY_SENSORS
        assert binary_sensor.CONF_PV2_SUPPORT in binary_sensor.BINARY_SENSORS
        assert len(binary_sensor.BINARY_SENSORS) == 3


class TestTextSensorConstants:
    def test_text_sensors_dict(self):
        assert text_sensor.CONF_SERIAL_NUMBER in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_OPERATION_MODE in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_DEVICE_MODEL in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 3


class TestButtonConstants:
    def test_buttons_dict(self):
        assert button.CONF_BATTERY_SETTINGS_RESET in button.BUTTONS
        assert len(button.BUTTONS) == 1

    def test_button_addresses_are_unique(self):
        addresses = list(button.BUTTONS.values())
        assert len(addresses) == len(set(addresses))


class TestNumberConstants:
    def test_numbers_dict(self):
        assert number.CONF_POWER_OUTPUT_SETTING in number.NUMBERS
        assert number.CONF_EQUALIZATION_VOLTAGE_SETTING in number.NUMBERS
        assert number.CONF_CHARGING_TARGET_VOLTAGE_SETTING in number.NUMBERS
        assert number.CONF_FLOAT_CHARGE_VOLTAGE_SETTING in number.NUMBERS
        assert number.CONF_BATTERY_CAPACITY_SETTING in number.NUMBERS
        assert len(number.NUMBERS) == 5

    def test_number_tuple_structure(self):
        for val in number.NUMBERS.values():
            register, factor, length = val
            assert isinstance(register, int)
            assert factor > 0
            assert isinstance(length, int)


class TestSwitchConstants:
    def test_switches_dict(self):
        assert switch.CONF_AC_CHARGING in switch.SWITCHES
        assert len(switch.SWITCHES) == 2

    def test_switch_addresses_are_unique(self):
        addresses = list(switch.SWITCHES.values())
        assert len(addresses) == len(set(addresses))


class TestSelectConstants:
    def test_operation_mode_options(self):
        assert "Battery Mode" in select_mod.OPERATION_MODE_OPTIONS
        assert "Hybrid Mode" in select_mod.OPERATION_MODE_OPTIONS
        assert "Grid-Tie Mode" in select_mod.OPERATION_MODE_OPTIONS
        assert len(select_mod.OPERATION_MODE_OPTIONS) == 3

    def test_operation_mode_values_are_unique(self):
        values = list(select_mod.OPERATION_MODE_OPTIONS.values())
        assert len(values) == len(set(values))
