#include "down_switch.h"

namespace esphome {
namespace vent_axia_sentinel_kinetic {

void DownSwitch::write_state(bool state) {
  this->publish_state(state);
  this->parent_->set_down(state);
}

}  // namespace vent_axia_sentinel_kinetic
}  // namespace esphome
