import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import CONF_ID

from .. import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CODEOWNERS = ["@syssi"]

CONF_AC_CHARGING = "ac_charging"
CONF_OUTPUT_ENABLE = "output_enable"

ICON_AC_CHARGING = "mdi:battery-charging-outline"
ICON_OUTPUT_ENABLE = "mdi:power"

SWITCHES = {
    CONF_AC_CHARGING: 120,
    CONF_OUTPUT_ENABLE: 121,
}

LumentreeSwitch = lumentree_ble_ns.class_(
    "LumentreeSwitch", switch.Switch, cg.Component
)

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_AC_CHARGING): switch.switch_schema(
            LumentreeSwitch,
            icon=ICON_AC_CHARGING,
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_OUTPUT_ENABLE): switch.switch_schema(
            LumentreeSwitch,
            icon=ICON_OUTPUT_ENABLE,
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for key, address in SWITCHES.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await switch.register_switch(var, conf)
            cg.add(getattr(hub, f"set_{key}_switch")(var))
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))
