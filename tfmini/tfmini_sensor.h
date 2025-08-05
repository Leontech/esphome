#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace tfmini_sensor {

class TFminiSensor : public PollingComponent, public sensor::Sensor, public uart::UARTDevice {
 public:
  TFminiSensor(uart::UARTComponent *parent) : PollingComponent(1, true), uart::UARTDevice(parent) {}

  void update() override;

  float get_setup_priority() const override { return setup_priority::DATA; }
};

}  // namespace tfmini_sensor
}  // namespace esphome
