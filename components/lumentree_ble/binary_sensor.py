import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import DEVICE_CLASS_CONNECTIVITY

from . import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CONF_GRID_CONNECTED = "grid_connected"
CONF_BATTERY_CONNECTED = "battery_connected"
CONF_PV2_SUPPORTED = "pv2_supported"

LumentreeBle = lumentree_ble_ns.class_("LumentreeBle")

BINARY_SENSORS = {
    CONF_GRID_CONNECTED: binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_CONNECTIVITY,
    ),
    CONF_BATTERY_CONNECTED: binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_CONNECTIVITY,
    ),
    CONF_PV2_SUPPORTED: binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_CONNECTIVITY,
    ),
}

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(sensor_name): sensor_config
        for sensor_name, sensor_config in BINARY_SENSORS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for sensor_name in BINARY_SENSORS:
        if sensor_name in config:
            conf = config[sensor_name]
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{sensor_name}_binary_sensor")(sens))
