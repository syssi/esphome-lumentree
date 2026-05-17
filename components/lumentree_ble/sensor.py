import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_BATTERY_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_KILOWATT_HOURS,
    UNIT_PERCENT,
    UNIT_VOLT,
    UNIT_VOLT_AMPS,
    UNIT_WATT,
)

from . import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]
CODEOWNERS = ["@syssi"]

# CONF_BATTERY_VOLTAGE = "battery_voltage"
CONF_BATTERY_CURRENT = "battery_current"
CONF_BATTERY_POWER = "battery_power"
CONF_BATTERY_SOC = "battery_soc"
CONF_AC_OUTPUT_VOLTAGE = "ac_output_voltage"
CONF_AC_INPUT_VOLTAGE = "ac_input_voltage"
CONF_AC_OUTPUT_FREQUENCY = "ac_output_frequency"
CONF_AC_INPUT_FREQUENCY = "ac_input_frequency"
CONF_AC_POWER = "ac_power"
CONF_PV_VOLTAGE = "pv_voltage"
CONF_PV_POWER = "pv_power"
CONF_GRID_POWER = "grid_power"
CONF_LOAD_POWER = "load_power"
CONF_DEVICE_TEMPERATURE = "device_temperature"
CONF_PV2_VOLTAGE = "pv2_voltage"
CONF_PV2_POWER = "pv2_power"
CONF_DEVICE_TYPE_IMAGE = "device_type_image"
CONF_BATTERY_STATUS = "battery_status"
CONF_GRID_CONNECTION_STATUS = "grid_connection_status"
CONF_DEVICE_TYPE = "device_type"
CONF_DEVICE_POWER_RATING_CODE = "device_power_rating_code"
CONF_DEVICE_POWER_RATING = "device_power_rating"
CONF_AC_OUTPUT_APPARENT_POWER = "ac_output_apparent_power"
CONF_GRID_CT_POWER = "grid_ct_power"
CONF_TODAY_PV_PRODUCTION = "today_pv_production"
CONF_TODAY_ESSENTIAL_LOAD = "today_essential_load"
CONF_TODAY_TOTAL_CONSUMPTION = "today_total_consumption"
CONF_TODAY_GRID_CONSUMPTION = "today_grid_consumption"
CONF_TODAY_BATTERY_CHARGING = "today_battery_charging"
CONF_TODAY_BATTERY_DISCHARGE = "today_battery_discharge"
CONF_GRID_EXPORT = "grid_export"

LumentreeBle = lumentree_ble_ns.class_("LumentreeBle")

SENSOR_DEFS = {
    CONF_BATTERY_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BATTERY_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BATTERY_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BATTERY_SOC: {
        "unit_of_measurement": UNIT_PERCENT,
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_OUTPUT_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_INPUT_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_OUTPUT_FREQUENCY: {
        "unit_of_measurement": UNIT_HERTZ,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_FREQUENCY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_INPUT_FREQUENCY: {
        "unit_of_measurement": UNIT_HERTZ,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_FREQUENCY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_PV_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_PV_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_GRID_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_LOAD_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DEVICE_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_PV2_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_PV2_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DEVICE_TYPE_IMAGE: {
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_BATTERY_STATUS: {
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_GRID_CONNECTION_STATUS: {
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DEVICE_TYPE: {
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DEVICE_POWER_RATING_CODE: {
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_DEVICE_POWER_RATING: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_OUTPUT_APPARENT_POWER: {
        "unit_of_measurement": UNIT_VOLT_AMPS,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_GRID_CT_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_TODAY_PV_PRODUCTION: {
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_TODAY_ESSENTIAL_LOAD: {
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_TODAY_TOTAL_CONSUMPTION: {
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_TODAY_GRID_CONSUMPTION: {
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_TODAY_BATTERY_CHARGING: {
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_TODAY_BATTERY_DISCHARGE: {
        "unit_of_measurement": UNIT_KILOWATT_HOURS,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
    },
    CONF_GRID_EXPORT: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
}

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {cv.Optional(key): sensor.sensor_schema(**kwargs) for key, kwargs in SENSOR_DEFS.items()}
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for key in SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
