#include "up_switch.h"

namespace esphome {
namespace vent_axia_sentinel_kinetic {

void UpSwitch::write_state(bool state) {
  this->publish_state(state);
  this->parent_->set_up(state);
}

}  // namespace vent_axia_sentinel_kinetic
}  // namespace esphome
