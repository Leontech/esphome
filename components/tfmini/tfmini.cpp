#include "tfmini.h"
#include "esphome/core/log.h"


namespace esphome {
namespace tfmini {

static const char *const TAG = "tfmini";
static const uint8_t HEADER = 0x59;  // frame header of data package

// Konstruktor nastavuje interval aktualizace na 1000 ms (60 sekunda)
TFMiniSensor::TFMiniSensor(uart::UARTComponent *parent) : PollingComponent(60000), UARTDevice(parent) {}

void TFMiniSensor::setup() {
  this->set_timeout(50, [this]() { this->setup_internal_(); });
}

void TFMiniSensor::setup_internal_() {
  const uint8_t standard_mode[5] = {0x5A, 0x05, 0x05, 0x01, 0x65};
  this->write_array(standard_mode, 5);
  delay(100);

  const uint8_t rate100[6] = {0x5A, 0x06, 0x03, 0x0A, 0x00, 0x6D};
  this->write_array(rate100, 6);
  delay(100);

  const uint8_t save_setting[4] = {0x5A, 0x04, 0x11, 0x6F};
  this->write_array(save_setting, 4);
  delay(1000);

  this->flush();
  ESP_LOGD(TAG, "TFMini setup completed");
}

// update() se volá každou sekundu, díky PollingComponent(1000)
void TFMiniSensor::update() {
  while (this->available() >= 9) {
    if (this->read() != HEADER) {
      continue;
    }
    if (this->read() != HEADER) {
      continue;
    }

    uint8_t data[7];
    if (!this->read_array(data, 7)) {
      continue;
    }

    uint8_t checksum = HEADER + HEADER;
    for (size_t i = 0; i < 6; i++) {
      checksum += data[i];
    }

    if (checksum != data[6]) {
      ESP_LOGW(TAG, "TFMini checksum error");
      continue;
    }

    uint16_t distance_cm = data[0] | (data[1] << 8);
    uint16_t strength = data[2] | (data[3] << 8);
    float temperature = (data[4] | (data[5] << 8)) / 8.0f - 256.0f;

    if (strength < 100) {
      ESP_LOGW(TAG, "TFMini signal strength too low: %u", strength);
      continue;
    }

    float distance_value;
    if (this->distance_unit_ == METERS) {
      distance_value = distance_cm / 100.0f;
      ESP_LOGD(TAG, "Distance: %.2f m, Strength: %u, Temperature: %.1f °C",
               distance_value, strength, temperature);
    } else {
      distance_value = distance_cm;
      ESP_LOGD(TAG, "Distance: %d cm, Strength: %u, Temperature: %.1f °C",
               distance_cm, strength, temperature);
    }

    this->publish_state(distance_value);
    if (this->strength_sensor_ != nullptr) {
      this->strength_sensor_->publish_state(strength);
    }
    if (this->temperature_sensor_ != nullptr) {
      this->temperature_sensor_->publish_state(temperature);
    }

    // Zpracujeme pouze první platný rámec na toto volání update()
    return;
  }
}

void TFMiniSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "TFMini:");
  LOG_SENSOR("  ", "Distance", this);
  ESP_LOGCONFIG(TAG, "  Distance Unit: %s", this->distance_unit_ == METERS ? "meters" : "centimeters");
  LOG_SENSOR("  ", "Signal Strength", this->strength_sensor_);
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  this->check_uart_settings(115200);
}

}  // namespace tfmini
}  // namespace esphome
