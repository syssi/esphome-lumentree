#include "lumentree_switch.h"

namespace esphome::lumentree_ble {

void LumentreeSwitch::write_state(bool state) {
  this->parent_->write_register(this->holding_register_, state ? 0x0001 : 0x0000);
}

}  // namespace esphome::lumentree_ble
