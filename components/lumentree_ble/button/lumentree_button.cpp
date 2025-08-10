#include "lumentree_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace lumentree_ble {

static const char *const TAG = "lumentree_ble.button";

void LumentreeButton::dump_config() { LOG_BUTTON("", "LumentreeBle Button", this); }
void LumentreeButton::press_action() {
  if (this->holding_register_ == 100) {
    // Battery Settings Reset: Write 5 zeros to registers 100-104
    this->parent_->write_multiple_registers(this->holding_register_, {0, 0, 0, 0, 0});
  } else {
    // Default: write single register
    this->parent_->write_register(this->holding_register_, 0x0001);
  }
}

}  // namespace lumentree_ble
}  // namespace esphome
