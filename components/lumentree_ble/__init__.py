import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]

AUTO_LOAD = ["binary_sensor", "sensor", "text_sensor"]
MULTI_CONF = True

CONF_LUMENTREE_BLE_ID = "lumentree_ble_id"

lumentree_ble_ns = cg.esphome_ns.namespace("lumentree_ble")
LumentreeBle = lumentree_ble_ns.class_(
    "LumentreeBle", ble_client.BLEClientNode, cg.PollingComponent
)

LUMENTREE_BLE_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_LUMENTREE_BLE_ID): cv.use_id(LumentreeBle),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(LumentreeBle),
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.polling_component_schema("30s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
