#include "lumentree_select.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lumentree_ble {

static const char *const TAG = "lumentree_ble.select";

void LumentreeSelect::dump_config() {
  LOG_SELECT("", "LumentreeBle Select", this);
  ESP_LOGCONFIG(TAG, "  Register: %u", this->holding_register_);
}

void LumentreeSelect::control(const std::string &value) {
  auto it = this->option_mappings_.find(value);
  if (it != this->option_mappings_.end()) {
    ESP_LOGD(TAG, "Setting %s to value %u on register %u", value.c_str(), it->second, this->holding_register_);
    this->parent_->write_register(this->holding_register_, it->second);
    this->publish_state(value);
  } else {
    ESP_LOGW(TAG, "Invalid option: %s", value.c_str());
  }
}

}  // namespace lumentree_ble
}  // namespace esphome