#pragma once

#include "esphome/components/switch/switch.h"
#include "../vent_axia_sentinel_kinetic.h"

namespace esphome {
namespace vent_axia_sentinel_kinetic {

class SetSwitch : public switch_::Switch, public Parented<VentAxiaSentinelKineticComponent> {
 public:
  SetSwitch() = default;

 protected:
  void write_state(bool state) override;
};

}  // namespace vent_axia_sentinel_kinetic
}  // namespace esphome
