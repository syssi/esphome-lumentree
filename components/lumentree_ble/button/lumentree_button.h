#pragma once

#include "../lumentree_ble.h"
#include "esphome/components/button/button.h"

namespace esphome::lumentree_ble {

class LumentreeBle;
class LumentreeButton : public button::Button {
 public:
  void set_parent(LumentreeBle *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };

 protected:
  void press_action() override;
  LumentreeBle *parent_;
  uint8_t holding_register_;
};

}  // namespace esphome::lumentree_ble
