import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_SWITCH,
    ENTITY_CATEGORY_CONFIG,
    ICON_PULSE,
)

from .. import CONF_VentAxiaSentinelKinetic_ID, VentAxiaSentinelKineticComponent, vent_axia_sentinel_kinetic_ns

UpSwitch = vent_axia_sentinel_kinetic_ns.class_("UpSwitch", switch.Switch)
DownSwitch = vent_axia_sentinel_kinetic_ns.class_("DownSwitch", switch.Switch)
SetSwitch = vent_axia_sentinel_kinetic_ns.class_("SetSwitch", switch.Switch)
MainSwitch = vent_axia_sentinel_kinetic_ns.class_("MainSwitch", switch.Switch)

CONF_UP = "up"
CONF_DOWN = "down"
CONF_SET = "set"
CONF_MAIN = "main"

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_VentAxiaSentinelKinetic_ID): cv.use_id(VentAxiaSentinelKineticComponent),
    cv.Optional(CONF_UP): switch.switch_schema(
        UpSwitch,
        device_class=DEVICE_CLASS_SWITCH,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_PULSE,
    ),
    cv.Optional(CONF_DOWN): switch.switch_schema(
        DownSwitch,
        device_class=DEVICE_CLASS_SWITCH,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_PULSE,
    ),
    cv.Optional(CONF_SET): switch.switch_schema(
        SetSwitch,
        device_class=DEVICE_CLASS_SWITCH,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_PULSE,
    ),
    cv.Optional(CONF_MAIN): switch.switch_schema(
        MainSwitch,
        device_class=DEVICE_CLASS_SWITCH,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_PULSE,
    ),
}


async def to_code(config):
    vent_axia_sentinel_kinetic_component = await cg.get_variable(config[CONF_VentAxiaSentinelKinetic_ID])
    if up_config := config.get(CONF_UP):
        s = await switch.new_switch(up_config)
        await cg.register_parented(s, config[CONF_VentAxiaSentinelKinetic_ID])
        cg.add(vent_axia_sentinel_kinetic_component.set_up_switch(s))
    if down_config := config.get(CONF_DOWN):
        s = await switch.new_switch(down_config)
        await cg.register_parented(s, config[CONF_VentAxiaSentinelKinetic_ID])
        cg.add(vent_axia_sentinel_kinetic_component.set_down_switch(s))
    if set_config := config.get(CONF_SET):
        s = await switch.new_switch(set_config)
        await cg.register_parented(s, config[CONF_VentAxiaSentinelKinetic_ID])
        cg.add(vent_axia_sentinel_kinetic_component.set_set_switch(s))
    if main_config := config.get(CONF_MAIN):
        s = await switch.new_switch(main_config)
        await cg.register_parented(s, config[CONF_VentAxiaSentinelKinetic_ID])
        cg.add(vent_axia_sentinel_kinetic_component.set_main_switch(s))
