#include "lumentree_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::lumentree_ble {

ESPHOME_LOG_TAG(TAG, "lumentree_ble.switch");

void LumentreeSwitch::dump_config() { LOG_SWITCH("", "LumentreeBle Switch", this); }

void LumentreeSwitch::write_state(bool state) {
  this->parent_->write_register(this->holding_register_, state ? 0x0001 : 0x0000);
}

}  // namespace esphome::lumentree_ble
