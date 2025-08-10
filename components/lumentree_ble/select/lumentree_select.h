#pragma once

#include "esphome/core/component.h"
#include "esphome/components/select/select.h"
#include "../lumentree_ble.h"

namespace esphome {
namespace lumentree_ble {

class LumentreeSelect : public select::Select, public Component {
 public:
  void set_parent(LumentreeBle *parent) { parent_ = parent; }
  void set_holding_register(uint16_t holding_register) { holding_register_ = holding_register; }
  void add_option_mapping(const std::string &option, uint16_t value) { option_mappings_[option] = value; }
  void dump_config() override;

 protected:
  void control(const std::string &value) override;

  LumentreeBle *parent_;
  uint16_t holding_register_;
  std::map<std::string, uint16_t> option_mappings_;
};

}  // namespace lumentree_ble
}  // namespace esphome