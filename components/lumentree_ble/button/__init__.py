import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_ID

from .. import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CODEOWNERS = ["@syssi"]

CONF_BATTERY_SETTINGS_RESET = "battery_settings_reset"

ICON_BATTERY_SETTINGS_RESET = "mdi:battery-sync"

BUTTONS = {
    CONF_BATTERY_SETTINGS_RESET: 100,
}

LumentreeButton = lumentree_ble_ns.class_(
    "LumentreeButton", button.Button, cg.Component
)

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_BATTERY_SETTINGS_RESET): button.button_schema(
            LumentreeButton,
            icon=ICON_BATTERY_SETTINGS_RESET,
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for key, address in BUTTONS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await button.register_button(var, conf)
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))
