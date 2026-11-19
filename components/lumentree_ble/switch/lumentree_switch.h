#pragma once

#include "../lumentree_ble.h"
#include "esphome/components/switch/switch.h"

namespace esphome::lumentree_ble {

class LumentreeBle;
class LumentreeSwitch : public switch_::Switch {
 public:
  void set_parent(LumentreeBle *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };

 protected:
  void write_state(bool state) override;
  LumentreeBle *parent_;
  uint8_t holding_register_;
};

}  // namespace esphome::lumentree_ble
