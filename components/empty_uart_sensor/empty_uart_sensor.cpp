#include "esphome/core/log.h"
#include "empty_uart_sensor.h"

hw_timer_t * timer = nullptr;

namespace esphome {
namespace empty_uart_sensor {

static const char *TAG = "empty_uart_sensor.sensor";

// Static member declarations
EmptyUARTSensor* EmptyUARTSensor::instance = nullptr;  // Static definition

void IRAM_ATTR EmptyUARTSensor::timer_isr_wrapper() {
  if (EmptyUARTSensor::instance) {  // Access through class name
    EmptyUARTSensor::instance->timer_isr();
  }
}

void IRAM_ATTR EmptyUARTSensor::timer_isr() {
  // Your ISR logic here
    digitalWrite(2,!(digitalRead(2)));  //Toggle LED Pin
}

void EmptyUARTSensor::setup() {
  instance = this;
  // timer = timerBegin(1, 1000000, true);
  // timerAttachInterrupt(timer, &EmptyUARTSensor::timer_isr_wrapper, true);
  // Fix timer divider (80 MHz base clock / 80 = 1 MHz)
  timer = timerBegin(1, 80, true);
  timerAttachInterrupt(timer, &timer_isr_wrapper, true);
  timerAlarmWrite(timer, 26000, true);  // 1 second interval
  timerAlarmEnable(timer);
  
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void EmptyUARTSensor::update() {

}

void EmptyUARTSensor::loop() {
  //Just a stupidly long delay to confirm the ISR is working
  delay(1000);
}

void EmptyUARTSensor::dump_config(){
    ESP_LOGCONFIG(TAG, "Empty UART sensor");
}

}  // namespace empty_UART_sensor
}  // namespace esphome