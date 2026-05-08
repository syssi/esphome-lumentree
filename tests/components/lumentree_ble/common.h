#pragma once
#include "esphome/components/lumentree_ble/lumentree_ble.h"

namespace esphome::lumentree_ble::testing {

class TestableLumentreeBle : public LumentreeBle {
 public:
  void update() override {}
};

}  // namespace esphome::lumentree_ble::testing
