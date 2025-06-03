import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import (
    uart,
    text_sensor,
    button,
)  # binary_sensor, output, sensor, switch,
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
AUTO_LOAD = ["text_sensor", 'button']  # 'binary_sensor', 'output', 'sensor',  'switch',
MULTI_CONF = True

vent_axia_sentinel_kinetic_ns = cg.esphome_ns.namespace("vent_axia_sentinel_kinetic")
VentAxiaSentinelKineticComponent = vent_axia_sentinel_kinetic_ns.class_(
    "VentAxiaSentinelKineticComponent", cg.Component, uart.UARTDevice
)
DiagnosticButton = vent_axia_sentinel_kinetic_ns.class_("DiagnosticButton", button.Button, cg.Component)

CONF_VentAxiaSentinelKinetic_ID = "vent_axia_sentinel_kinetic_id"

CONF_DIAGNOSTIC_BUTTON = "diagnostic_button"

CONF_LINE1 = "line1"
CONF_LINE2 = "line2"
CONF_DIAGNOSTIC0 = "diagnostic0"
CONF_DIAGNOSTIC1 = "diagnostic1"
CONF_DIAGNOSTIC2 = "diagnostic2"
CONF_DIAGNOSTIC3 = "diagnostic3"
CONF_DIAGNOSTIC4 = "diagnostic4"
CONF_DIAGNOSTIC5 = "diagnostic5"
CONF_DIAGNOSTIC6 = "diagnostic6"
CONF_DIAGNOSTIC7 = "diagnostic7"
CONF_DIAGNOSTIC8 = "diagnostic8"
CONF_DIAGNOSTIC9 = "diagnostic9"
CONF_DIAGNOSTIC10 = "diagnostic10"
CONF_DIAGNOSTIC11 = "diagnostic11"
CONF_DIAGNOSTIC12 = "diagnostic12"
CONF_DIAGNOSTIC13 = "diagnostic13"
CONF_DIAGNOSTIC14 = "diagnostic14"
CONF_DIAGNOSTIC15 = "diagnostic15"
CONF_DIAGNOSTIC16 = "diagnostic16"
CONF_DIAGNOSTIC17 = "diagnostic17"
CONF_DIAGNOSTIC18 = "diagnostic18"
CONF_DIAGNOSTIC19 = "diagnostic19"
CONF_DIAGNOSTIC20 = "diagnostic20"
CONF_DIAGNOSTIC21 = "diagnostic21"
CONF_DIAGNOSTIC22 = "diagnostic22"
CONF_DIAGNOSTIC23 = "diagnostic23"
CONF_DIAGNOSTIC24 = "diagnostic24"
CONF_DIAGNOSTIC25 = "diagnostic25"
CONF_DIAGNOSTIC26 = "diagnostic26"
CONF_DIAGNOSTIC27 = "diagnostic27"
CONF_DIAGNOSTIC28 = "diagnostic28"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(VentAxiaSentinelKineticComponent),
            cv.Optional(CONF_DIAGNOSTIC_BUTTON): button.BUTTON_SCHEMA.extend({cv.GenerateID(): cv.declare_id(DiagnosticButton)}),
            cv.Optional(CONF_LINE1): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_LINE2): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC0): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC1): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC2): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC3): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC4): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC5): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC6): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC7): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC8): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC9): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC10): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC11): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC12): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC13): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC14): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC15): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC16): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC17): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC18): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC19): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC20): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC21): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC22): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC23): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC24): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC25): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC26): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC27): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
            cv.Optional(CONF_DIAGNOSTIC28): text_sensor.text_sensor_schema(
                text_sensor.TextSensor
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "vent_axia_sentinel_kinetic",
    require_tx=True,
    require_rx=True,
    parity="NONE",
    stop_bits=1,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_DIAGNOSTIC_BUTTON in config:
        btn = await button.new_button(config[CONF_DIAGNOSTIC_BUTTON])
        cg.add(btn.set_parent(var))

    if CONF_LINE1 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_LINE1])
        cg.add(var.set_line1(sens))

    if CONF_LINE2 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_LINE2])
        cg.add(var.set_line2(sens))

    if CONF_DIAGNOSTIC0 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC0])
        cg.add(var.set_diagnostic0(sens))

    if CONF_DIAGNOSTIC1 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC1])
        cg.add(var.set_diagnostic1(sens))

    if CONF_DIAGNOSTIC2 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC2])
        cg.add(var.set_diagnostic2(sens))

    if CONF_DIAGNOSTIC3 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC3])
        cg.add(var.set_diagnostic3(sens))

    if CONF_DIAGNOSTIC4 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC4])
        cg.add(var.set_diagnostic4(sens))

    if CONF_DIAGNOSTIC5 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC5])
        cg.add(var.set_diagnostic5(sens))

    if CONF_DIAGNOSTIC6 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC6])
        cg.add(var.set_diagnostic6(sens))

    if CONF_DIAGNOSTIC7 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC7])
        cg.add(var.set_diagnostic7(sens))

    if CONF_DIAGNOSTIC8 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC8])
        cg.add(var.set_diagnostic8(sens))

    if CONF_DIAGNOSTIC9 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC9])
        cg.add(var.set_diagnostic9(sens))

    if CONF_DIAGNOSTIC10 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC10])
        cg.add(var.set_diagnostic10(sens))

    if CONF_DIAGNOSTIC11 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC11])
        cg.add(var.set_diagnostic11(sens))

    if CONF_DIAGNOSTIC12 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC12])
        cg.add(var.set_diagnostic12(sens))

    if CONF_DIAGNOSTIC13 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC13])
        cg.add(var.set_diagnostic13(sens))

    if CONF_DIAGNOSTIC14 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC14])
        cg.add(var.set_diagnostic14(sens))

    if CONF_DIAGNOSTIC15 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC15])
        cg.add(var.set_diagnostic15(sens))

    if CONF_DIAGNOSTIC16 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC16])
        cg.add(var.set_diagnostic16(sens))

    if CONF_DIAGNOSTIC17 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC17])
        cg.add(var.set_diagnostic17(sens))

    if CONF_DIAGNOSTIC18 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC18])
        cg.add(var.set_diagnostic18(sens))

    if CONF_DIAGNOSTIC19 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC19])
        cg.add(var.set_diagnostic19(sens))

    if CONF_DIAGNOSTIC20 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC20])
        cg.add(var.set_diagnostic20(sens))

    if CONF_DIAGNOSTIC21 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC21])
        cg.add(var.set_diagnostic21(sens))

    if CONF_DIAGNOSTIC22 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC22])
        cg.add(var.set_diagnostic22(sens))

    if CONF_DIAGNOSTIC23 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC23])
        cg.add(var.set_diagnostic23(sens))

    if CONF_DIAGNOSTIC24 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC24])
        cg.add(var.set_diagnostic24(sens))

    if CONF_DIAGNOSTIC25 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC25])
        cg.add(var.set_diagnostic25(sens))

    if CONF_DIAGNOSTIC26 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC26])
        cg.add(var.set_diagnostic26(sens))

    if CONF_DIAGNOSTIC27 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC27])
        cg.add(var.set_diagnostic27(sens))

    if CONF_DIAGNOSTIC28 in config:
        sens = await text_sensor.new_text_sensor(config[CONF_DIAGNOSTIC28])
        cg.add(var.set_diagnostic28(sens))
