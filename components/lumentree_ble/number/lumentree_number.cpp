#include "lumentree_number.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::lumentree_ble {

ESPHOME_LOG_TAG(TAG, "lumentree_ble.number");

void LumentreeNumber::dump_config() { LOG_NUMBER("", "LumentreeBle Number", this); }

void LumentreeNumber::control(float value) {
  this->parent_->write_register(this->holding_register_, (uint16_t) (value / this->factor_));
}

}  // namespace esphome::lumentree_ble
