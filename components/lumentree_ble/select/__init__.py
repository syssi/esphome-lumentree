import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv

from .. import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CODEOWNERS = ["@syssi"]

CONF_OPERATION_MODE = "operation_mode"

OPERATION_MODE_OPTIONS = {
    "Battery Mode": 0,
    "Hybrid Mode": 1,
    "Grid-Tie Mode": 2,
}

LumentreeSelect = lumentree_ble_ns.class_(
    "LumentreeSelect", select.Select, cg.Component
)

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_OPERATION_MODE): select.select_schema(
            LumentreeSelect,
            icon="mdi:cog",
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    if CONF_OPERATION_MODE in config:
        conf = config[CONF_OPERATION_MODE]
        var = cg.new_Pvariable(conf[cv.CONF_ID])
        await cg.register_component(var, conf)
        await select.register_select(
            var, conf, options=list(OPERATION_MODE_OPTIONS.keys())
        )
        cg.add(var.set_parent(hub))
        cg.add(var.set_holding_register(150))  # Work Mode Setting register

        # Add option mapping
        for option, value in OPERATION_MODE_OPTIONS.items():
            cg.add(var.add_option_mapping(option, value))

        # Register with hub for sync with register 68
        cg.add(hub.set_operation_mode_select(var))
