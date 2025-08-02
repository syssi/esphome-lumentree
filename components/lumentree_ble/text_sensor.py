import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CONF_DEVICE_MODEL = "device_model"
CONF_OPERATION_MODE = "operation_mode"

LumentreeBle = lumentree_ble_ns.class_("LumentreeBle")

TEXT_SENSORS = {
    CONF_DEVICE_MODEL: text_sensor.text_sensor_schema(
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    CONF_OPERATION_MODE: text_sensor.text_sensor_schema(),
}

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(sensor_name): sensor_config
        for sensor_name, sensor_config in TEXT_SENSORS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for sensor_name in TEXT_SENSORS:
        if sensor_name in config:
            conf = config[sensor_name]
            sens = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(hub, f"set_{sensor_name}_text_sensor")(sens))
