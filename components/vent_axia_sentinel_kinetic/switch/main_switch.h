#pragma once

#include "esphome/components/switch/switch.h"
#include "../vent_axia_sentinel_kinetic.h"

namespace esphome {
namespace vent_axia_sentinel_kinetic {

class MainSwitch : public switch_::Switch, public Parented<VentAxiaSentinelKineticComponent> {
 public:
  MainSwitch() = default;

 protected:
  void write_state(bool state) override;
};

}  // namespace vent_axia_sentinel_kinetic
}  // namespace esphome
