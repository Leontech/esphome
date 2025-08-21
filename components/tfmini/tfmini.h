#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace tfmini {

enum DistanceUnit {
  CENTIMETERS,
  METERS
};

class TFMiniSensor : public sensor::Sensor, public PollingComponent, public uart::UARTDevice {
 public:
  TFMiniSensor(UARTComponent *parent);

  void setup() override;
  void update() override;
  void dump_config() override;

  void set_distance_unit(DistanceUnit unit) { this->distance_unit_ = unit; }
  void set_strength_sensor(sensor::Sensor *strength) { this->strength_sensor_ = strength; }
  void set_temperature_sensor(sensor::Sensor *temperature) { this->temperature_sensor_ = temperature; }

 protected:
  void setup_internal_();

  DistanceUnit distance_unit_{CENTIMETERS};
  sensor::Sensor *strength_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
};

}  // namespace tfmini
}  // namespace esphome
