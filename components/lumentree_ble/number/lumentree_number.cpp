#include "lumentree_number.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace lumentree_ble {

static const char *const TAG = "lumentree_ble.number";

void LumentreeNumber::dump_config() { LOG_NUMBER("", "LumentreeBle Number", this); }

void LumentreeNumber::control(float value) {
  this->parent_->write_register(this->holding_register_, (uint16_t) (value / this->factor_));
}

}  // namespace lumentree_ble
}  // namespace esphome
