#include "main_switch.h"

namespace esphome {
namespace vent_axia_sentinel_kinetic {

void MainSwitch::write_state(bool state) {
  this->publish_state(state);
  this->parent_->set_main(state);
}

}  // namespace vent_axia_sentinel_kinetic
}  // namespace esphome
