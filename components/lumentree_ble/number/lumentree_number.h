#pragma once

#include "../lumentree_ble.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace lumentree_ble {

class LumentreeBle;
class LumentreeNumber : public number::Number, public Component {
 public:
  void set_parent(LumentreeBle *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };
  void set_factor(float factor) { this->factor_ = factor; };
  void set_length(uint8_t length) { this->length_ = length; };
  void dump_config() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void control(float value) override;
  LumentreeBle *parent_;
  uint8_t holding_register_;
  float factor_;
  uint8_t length_;
};

}  // namespace lumentree_ble
}  // namespace esphome
