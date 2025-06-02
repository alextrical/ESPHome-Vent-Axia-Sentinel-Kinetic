#pragma once

// #include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace empty_uart_sensor {

class EmptyUARTSensor : public sensor::Sensor, public PollingComponent, public uart::UARTDevice {
 public:
  void setup() override;
  void update() override;
  void loop() override;
  void dump_config() override;
  
  private:
    static EmptyUARTSensor* instance;  // Static member declaration
    void IRAM_ATTR timer_isr();
    static void IRAM_ATTR timer_isr_wrapper();
    hw_timer_t *timer = nullptr;
// #ifdef ESP8266
//   static void esp8266_timer_isr();
// #endif
};

}  // namespace empty_uart_sensor
}  // namespace esphome