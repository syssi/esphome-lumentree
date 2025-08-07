import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_MODE,
    CONF_STEP,
    CONF_UNIT_OF_MEASUREMENT,
    ENTITY_CATEGORY_CONFIG,
    ICON_EMPTY,
    UNIT_VOLT,
    UNIT_WATT,
)

from .. import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CODEOWNERS = ["@syssi"]

UNIT_AMPERE_HOUR = "Ah"

DEFAULT_STEP = 1

CONF_POWER_OUTPUT_SETTING = "power_output_setting"
CONF_EQUALIZATION_VOLTAGE_SETTING = "equalization_voltage_setting"
CONF_CHARGING_TARGET_VOLTAGE_SETTING = "charging_target_voltage_setting"
CONF_FLOAT_CHARGE_VOLTAGE_SETTING = "float_charge_voltage_setting"
CONF_BATTERY_CAPACITY_SETTING = "battery_capacity_setting"

NUMBERS = {
    CONF_POWER_OUTPUT_SETTING: [40, 1.0, 2],
    CONF_EQUALIZATION_VOLTAGE_SETTING: [101, 0.01, 2],
    CONF_CHARGING_TARGET_VOLTAGE_SETTING: [102, 0.01, 2],
    CONF_FLOAT_CHARGE_VOLTAGE_SETTING: [103, 0.01, 2],
    CONF_BATTERY_CAPACITY_SETTING: [104, 1.0, 2],
}

LumentreeNumber = lumentree_ble_ns.class_(
    "LumentreeNumber", number.Number, cg.Component
)

LUMENTREE_NUMBER_SCHEMA = (
    number.number_schema(
        LumentreeNumber,
        icon=ICON_EMPTY,
        entity_category=ENTITY_CATEGORY_CONFIG,
        unit_of_measurement=UNIT_VOLT,
    )
    .extend(
        {
            cv.Optional(CONF_STEP, default=0.01): cv.float_,
            cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                number.NUMBER_MODES, upper=True
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_POWER_OUTPUT_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=2000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_WATT
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_EQUALIZATION_VOLTAGE_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=48.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=60.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(
            CONF_CHARGING_TARGET_VOLTAGE_SETTING
        ): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=48.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=58.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_FLOAT_CHARGE_VOLTAGE_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=48.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=56.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_BATTERY_CAPACITY_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=50.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=1000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE_HOUR
                ): cv.string_strict,
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for key, address in NUMBERS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await number.register_number(
                var,
                conf,
                min_value=conf[CONF_MIN_VALUE],
                max_value=conf[CONF_MAX_VALUE],
                step=conf[CONF_STEP],
            )
            cg.add(getattr(hub, f"set_{key}_number")(var))
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address[0]))
            cg.add(var.set_factor(address[1]))
            cg.add(var.set_length(address[2]))
