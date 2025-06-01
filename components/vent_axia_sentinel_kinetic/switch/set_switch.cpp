#include "set_switch.h"

namespace esphome {
namespace vent_axia_sentinel_kinetic {

void SetSwitch::write_state(bool state) {
  this->publish_state(state);
  this->parent_->set_set(state);
}

}  // namespace vent_axia_sentinel_kinetic
}  // namespace esphome
