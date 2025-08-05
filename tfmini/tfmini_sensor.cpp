#include "tfmini_sensor.h"
#include "esphome/core/log.h"

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

}  // namespace tfmini_sensor
}  // namespace esphome

