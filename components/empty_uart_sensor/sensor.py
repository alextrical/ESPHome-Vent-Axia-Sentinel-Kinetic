import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import ICON_EMPTY, UNIT_EMPTY

DEPENDENCIES = ["uart"]

empty_uart_sensor_ns = cg.esphome_ns.namespace("empty_uart_sensor")
EmptyUARTSensor = empty_uart_sensor_ns.class_(
    "EmptyUARTSensor", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        EmptyUARTSensor,
        unit_of_measurement=UNIT_EMPTY,
        icon=ICON_EMPTY,
        accuracy_decimals=1,
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
