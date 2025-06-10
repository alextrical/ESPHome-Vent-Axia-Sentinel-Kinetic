#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/button/button.h"

#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

namespace esphome {
  namespace vent_axia_sentinel_kinetic {

    // Commands
    static
    const uint8_t CMD_FRAME_HEADER = 0x04;
    static
    const uint8_t CMD_ALIVE_HEADER[4] = {
      0x06,
      0xFF,
      0xFF,
      0xFF
    };
    static
    const uint8_t CMD_KEY_HEADER[4] = {
      0x05,
      0xAF,
      0xEF,
      0xFB
    };
    static
    const uint8_t CMD_ALIVE_DATA = 0x10;
    static uint8_t CMD_KEY_DATA = 0x00;

    class VentAxiaSentinelKineticComponent: public uart::UARTDevice, public Component {
      #ifdef USE_SWITCH
      SUB_SWITCH(up)
      SUB_SWITCH(down)
      SUB_SWITCH(set)
      SUB_SWITCH(main)
      #endif

      public:
        float get_setup_priority() const override {
          return setup_priority::LATE;
        }
        void setup() override;
        void loop() override;
        void dump_config() override;
        void set_up(bool enable);
        void set_down(bool enable);
        void set_set(bool enable);
        void set_main(bool enable);

        void set_line1(text_sensor::TextSensor * text_sensor) {line1_ = text_sensor;}
        void set_line2(text_sensor::TextSensor * text_sensor) {line2_ = text_sensor;}
        void set_diagnostic0(text_sensor::TextSensor * text_sensor) {diagnostic0_ = text_sensor;}
        void set_diagnostic1(text_sensor::TextSensor * text_sensor) {diagnostic1_ = text_sensor;}
        void set_diagnostic2(text_sensor::TextSensor * text_sensor) {diagnostic2_ = text_sensor;}
        void set_diagnostic3(text_sensor::TextSensor * text_sensor) {diagnostic3_ = text_sensor;}
        void set_diagnostic4(text_sensor::TextSensor * text_sensor) {diagnostic4_ = text_sensor;}
        void set_diagnostic5(text_sensor::TextSensor * text_sensor) {diagnostic5_ = text_sensor;}
        void set_diagnostic6(text_sensor::TextSensor * text_sensor) {diagnostic6_ = text_sensor;}
        void set_diagnostic7(text_sensor::TextSensor * text_sensor) {diagnostic7_ = text_sensor;}
        void set_diagnostic8(text_sensor::TextSensor * text_sensor) {diagnostic8_ = text_sensor;}
        void set_diagnostic9(text_sensor::TextSensor * text_sensor) {diagnostic9_ = text_sensor;}
        void set_diagnostic10(text_sensor::TextSensor * text_sensor) {diagnostic10_ = text_sensor;}
        void set_diagnostic11(text_sensor::TextSensor * text_sensor) {diagnostic11_ = text_sensor;}
        void set_diagnostic12(text_sensor::TextSensor * text_sensor) {diagnostic12_ = text_sensor;}
        void set_diagnostic13(text_sensor::TextSensor * text_sensor) {diagnostic13_ = text_sensor;}
        void set_diagnostic14(text_sensor::TextSensor * text_sensor) {diagnostic14_ = text_sensor;}
        void set_diagnostic15(text_sensor::TextSensor * text_sensor) {diagnostic15_ = text_sensor;}
        void set_diagnostic16(text_sensor::TextSensor * text_sensor) {diagnostic16_ = text_sensor;}
        void set_diagnostic17(text_sensor::TextSensor * text_sensor) {diagnostic17_ = text_sensor;}
        void set_diagnostic18(text_sensor::TextSensor * text_sensor) {diagnostic18_ = text_sensor;}
        void set_diagnostic19(text_sensor::TextSensor * text_sensor) {diagnostic19_ = text_sensor;}
        void set_diagnostic20(text_sensor::TextSensor * text_sensor) {diagnostic20_ = text_sensor;}
        void set_diagnostic21(text_sensor::TextSensor * text_sensor) {diagnostic21_ = text_sensor;}
        void set_diagnostic22(text_sensor::TextSensor * text_sensor) {diagnostic22_ = text_sensor;}
        void set_diagnostic23(text_sensor::TextSensor * text_sensor) {diagnostic23_ = text_sensor;}
        void set_diagnostic24(text_sensor::TextSensor * text_sensor) {diagnostic24_ = text_sensor;}
        void set_diagnostic25(text_sensor::TextSensor * text_sensor) {diagnostic25_ = text_sensor;}
        void set_diagnostic26(text_sensor::TextSensor * text_sensor) {diagnostic26_ = text_sensor;}
        void set_diagnostic27(text_sensor::TextSensor * text_sensor) {diagnostic27_ = text_sensor;}
        void set_diagnostic28(text_sensor::TextSensor * text_sensor) {diagnostic28_ = text_sensor;}

        uint8_t buffer[41];
        uint8_t last_buffer[41];
        bool get_diagnostic = false;

      protected:
        text_sensor::TextSensor * line1_ {nullptr};
        text_sensor::TextSensor * line2_ {nullptr};
        text_sensor::TextSensor * diagnostic0_ {nullptr};
        text_sensor::TextSensor * diagnostic1_ {nullptr};
        text_sensor::TextSensor * diagnostic2_ {nullptr};
        text_sensor::TextSensor * diagnostic3_ {nullptr};
        text_sensor::TextSensor * diagnostic4_ {nullptr};
        text_sensor::TextSensor * diagnostic5_ {nullptr};
        text_sensor::TextSensor * diagnostic6_ {nullptr};
        text_sensor::TextSensor * diagnostic7_ {nullptr};
        text_sensor::TextSensor * diagnostic8_ {nullptr};
        text_sensor::TextSensor * diagnostic9_ {nullptr};
        text_sensor::TextSensor * diagnostic10_ {nullptr};
        text_sensor::TextSensor * diagnostic11_ {nullptr};
        text_sensor::TextSensor * diagnostic12_ {nullptr};
        text_sensor::TextSensor * diagnostic13_ {nullptr};
        text_sensor::TextSensor * diagnostic14_ {nullptr};
        text_sensor::TextSensor * diagnostic15_ {nullptr};
        text_sensor::TextSensor * diagnostic16_ {nullptr};
        text_sensor::TextSensor * diagnostic17_ {nullptr};
        text_sensor::TextSensor * diagnostic18_ {nullptr};
        text_sensor::TextSensor * diagnostic19_ {nullptr};
        text_sensor::TextSensor * diagnostic20_ {nullptr};
        text_sensor::TextSensor * diagnostic21_ {nullptr};
        text_sensor::TextSensor * diagnostic22_ {nullptr};
        text_sensor::TextSensor * diagnostic23_ {nullptr};
        text_sensor::TextSensor * diagnostic24_ {nullptr};
        text_sensor::TextSensor * diagnostic25_ {nullptr};
        text_sensor::TextSensor * diagnostic26_ {nullptr};
        text_sensor::TextSensor * diagnostic27_ {nullptr};
        text_sensor::TextSensor * diagnostic28_ {nullptr};
        void calculate_command_(const uint8_t * command_value, uint8_t command_str);
        void send_alive_str_();
        void publishDiagnosticIfValid(text_sensor::TextSensor* diag, const std::string& buff);

        void send_command_();
        int32_t last_periodic_millis_ = millis();
        bool validate_crc_();
        void process_packet_();
        uint8_t current_index_ = 0;
        uint8_t cmdbuffer_[8];
        uint8_t LAST_CMD_KEY_DATA_ = 0x00;
        // bool packet_ready_ = false;

        int32_t toggle_time_;
        bool diagnostic_time_set_ = false;
        int32_t diagnostic_time_;
        bool got_diagnostic_ = false;

        static VentAxiaSentinelKineticComponent* instance;
        void IRAM_ATTR timer_isr();
        static void IRAM_ATTR timer_isr_wrapper();
#ifdef USE_ESP32
        hw_timer_t *timer = nullptr;
#endif
    };

class DiagnosticButton: public Component, public button::Button {
 public:
  void dump_config() override;
  void set_parent(VentAxiaSentinelKineticComponent *parent) { this->parent_ = parent; }
 protected:
  void press_action() override;
  VentAxiaSentinelKineticComponent *parent_;
};

  } // namespace vent_axia_sentinel_kinetic
} // namespace esphome