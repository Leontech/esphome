#include "tfmini_sensor.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace tfmini_sensor {

static const char *TAG = "tfmini_sensor";

void TFminiSensor::update() {
  int uart_value[9];
  const int HEADER = 0x59;
  float distance = 0;

  if (available() > 0) {
    if (read() == HEADER) {
      uart_value[0] = HEADER;
      if (read() == HEADER) uart_value[1] = HEADER;
      for (int i = 2; i < 9; i++) {
        uart_value[i] = read();
      }

      int check = 0;
      for (int i = 0; i < 8; i++) check += uart_value[i];

      if (uart_value[8] == (check & 0xff)) {
        distance = uart_value[2] + uart_value[3] * 256;
        publish_state(distance);  // v cm
      }
    }
  }
}

// Factory funkce pro vytvoření instance senzoru
sensor::Sensor *create_tfmini_sensor(UARTComponent *parent) {
  return new TFminiSensor(parent);
}

// Funkce, která registruje platformu "tfmini_sensor" do ESPHome
void register_tfmini_sensor() {
  static bool registered = false;
  if (!registered) {
    sensor::SensorFactory::get_instance().register_factory(
      "tfmini_sensor",
      [](const sensor::SensorFactory::PlatformContext &context) -> sensor::Sensor* {
        return new TFminiSensor(context.get_uart());
      });
    registered = true;
  }
}

// Automatické zavolání registrace při spuštění
const bool tfmini_sensor_registered = []() {
  register_tfmini_sensor();
  return true;
}();

}  // namespace tfmini_sensor
}  // namespace esphome
