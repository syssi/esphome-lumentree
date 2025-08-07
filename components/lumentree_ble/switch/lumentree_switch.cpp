#include "lumentree_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace lumentree_ble {

static const char *const TAG = "lumentree_ble.switch";

void LumentreeSwitch::dump_config() { LOG_SWITCH("", "LumentreeBle Switch", this); }

void LumentreeSwitch::write_state(bool state) {
  uint16_t register_value = state ? 0x0001 : 0x0000;
  this->parent_->write_register(this->holding_register_, register_value);
}

}  // namespace lumentree_ble
}  // namespace esphome