#include "lumentree_button.h"

namespace esphome::lumentree_ble {

void LumentreeButton::press_action() {
  if (this->holding_register_ == 100) {
    // Battery Settings Reset: Write 5 zeros to registers 100-104
    this->parent_->write_multiple_registers(this->holding_register_, {0, 0, 0, 0, 0});
  } else {
    // Default: write single register
    this->parent_->write_register(this->holding_register_, 0x0001);
  }
}

}  // namespace esphome::lumentree_ble
