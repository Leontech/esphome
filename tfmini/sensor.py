import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_UART_ID
from esphome.cpp_generator import Pvariable, add
from esphome.cpp_types import App, Component, UARTDevice, sensor as sensor_ns

TFminiSensor = sensor_ns.class_(
    "TFminiSensor", App.polling_component, sensor.Sensor, UARTDevice
)

PLATFORM_SCHEMA = sensor.SENSOR_PLATFORM_SCHEMA.extend(
    {
        cv.Required(CONF_UART_ID): cv.use_id(UARTDevice),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_UART_ID])
    sens = Pvariable(config[CONF_ID], TFminiSensor(parent))
    await sensor.register_sensor(sens, config)
    await cg.register_component(sens, config)
