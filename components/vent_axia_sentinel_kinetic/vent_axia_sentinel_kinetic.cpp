#include "esphome/core/log.h"
#include "vent_axia_sentinel_kinetic.h"

#define highbyte(val)(uint8_t)((val) >> 8)
#define lowbyte(val)(uint8_t)((val) & 0xff)

namespace esphome {
  namespace vent_axia_sentinel_kinetic {

    static
    const char * TAG = "vent_axia_sentinel_kinetic.component";

    VentAxiaSentinelKineticComponent* VentAxiaSentinelKineticComponent::instance = nullptr;

    void IRAM_ATTR VentAxiaSentinelKineticComponent::timer_isr_wrapper() {
      if (VentAxiaSentinelKineticComponent::instance) {
        VentAxiaSentinelKineticComponent::instance->timer_isr();
      }
    }

    void IRAM_ATTR VentAxiaSentinelKineticComponent::timer_isr() {
      // Your ISR logic here
        send_command_();
    }

    void VentAxiaSentinelKineticComponent::setup() {
      //ESP32
      instance = this;
    #ifdef USE_ESP32
      // Fix timer divider (80 MHz base clock / 80 = 1 MHz)
      timer = timerBegin(1, 80, true);
      timerAttachInterrupt(timer, &timer_isr_wrapper, true);
      timerAlarmWrite(timer, 26000, true);  // 1 second interval
      // timerAlarmEnable(timer);
    #endif
    #ifdef USE_ESP8266
      // Timer configuration
      timer1_disable();
      timer1_attachInterrupt(timer_isr_wrapper);
      timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
      timer1_disable();
      
      // 5,000,000 ticks @ 5MHz (TIM_DIV16) = 1 second interval
      // timer1_write(5*26000);
      timer1_write(5*28000);
    #endif
      this->send_alive_str_();
      ESP_LOGCONFIG(TAG, "VentAxiaSentinelKinetic setup complete.");
    }

    void VentAxiaSentinelKineticComponent::loop() {
      int32_t now = millis();
      if (this->get_diagnostic) {
        if (!this->diagnostic_time_set_) {
          this->diagnostic_time_ = now;
          this->toggle_time_ = now;
          this->diagnostic_time_set_ = true;
          this->got_diagnostic_ = false;
        }
        if (now - this->diagnostic_time_ <= 60000){ //timeout incase we don't enter diagnostic
          if (id(line1_).state.rfind("Diagnostic", 0) != 0 && !got_diagnostic_){ //We arent in the Diagnostic Menu, Hold Up and Main until we are
            if (CMD_KEY_DATA != 0x0A) CMD_KEY_DATA = 0x0A; //Up+Main
          } else if (!this->got_diagnostic_){ //Scroll down and Fetch all Diagnostic data
            if (now - this->toggle_time_ > 100) {
              if (CMD_KEY_DATA != 0x00){ //toggle
                CMD_KEY_DATA = 0x01; //Down
              } else {
                CMD_KEY_DATA = 0x00; //Release all
              }
              this->toggle_time_ = now;
            }
            if (id(line1_).state.rfind("Diagnostic  28", 0) == 0) this->got_diagnostic_ = true; //We are at the bottom entry od the Diagnostic menu, we have seen everything
          } else if (id(line1_).state.rfind("Diagnostic", 0) == 0 && this->got_diagnostic_){ //We have all Diagnostic data we want, Now exit
                if (now - this->toggle_time_ > 1000) {
                  CMD_KEY_DATA = 0x00; //Release all
                  this->toggle_time_ = now;
                } else if (CMD_KEY_DATA != 0x02) {
                  CMD_KEY_DATA = 0x02; //Up
                }
          } else { //We have exitetd the diagnostic menu, release all keys 
            if (CMD_KEY_DATA != 0x00) CMD_KEY_DATA = 0x00; //Release all
            this->get_diagnostic = false;
            this->diagnostic_time_set_ = false;
            this->got_diagnostic_ = false;
          }
        } else if (this->get_diagnostic) { //We timed out, release all keys
            if (CMD_KEY_DATA != 0x00) CMD_KEY_DATA = 0x00; //Release all
            this->get_diagnostic = false;
            this->diagnostic_time_set_ = false;
            this->got_diagnostic_ = false;
        }
      }

      //Send serial packets
      if (CMD_KEY_DATA != 0) {
        if (CMD_KEY_DATA != LAST_CMD_KEY_DATA_) {
          this->calculate_command_(CMD_KEY_HEADER, CMD_KEY_DATA);
          LAST_CMD_KEY_DATA_ = CMD_KEY_DATA;
        }
      #ifdef USE_ESP32
        timerAlarmEnable(timer);
      #endif
      #ifdef USE_ESP8266
        timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
      #endif
        // // //Send serial packets
        // if (now - last_periodic_millis_ >= 20) { //we need every 26ms, but setting less can allow for more jitter
        //   last_periodic_millis_ = now;
        //   send_command_();
        // }
      } else {
        if (CMD_KEY_DATA != LAST_CMD_KEY_DATA_) {
        #ifdef USE_ESP32
          timerAlarmDisable(timer);
        #endif
        #ifdef USE_ESP8266
          timer1_disable();
        #endif
        }
      }

      //Recieve Serial packets
      while (this->available() != 0) {
        uint8_t c;
        this->read_byte( & c);

        if (this->current_index_ == 0 && c != 0x02) {
          return; // Wait for header
        }

        this->buffer[this->current_index_++] = c;

        if (this->current_index_ == sizeof(this->buffer)) {
          if (this->validate_crc_()) {
            // this->packet_ready_ = true;
            this->process_packet_();
          }
          this->current_index_ = 0;
        }
      }

    }

    void VentAxiaSentinelKineticComponent::dump_config() {
      this->check_uart_settings(9600, 1, esphome::uart::UART_CONFIG_PARITY_NONE, 8);
      ESP_LOGCONFIG(TAG, "Vent Axia Sentinel Kinetic Component");
      if (this->is_failed()) {
        ESP_LOGE(TAG, "Connection with VentAxiaSentinelKinetic failed!");
      }
      LOG_TEXT_SENSOR("", "Line1 Sensor", this->line1_);
      LOG_TEXT_SENSOR("", "Line2 Sensor", this->line2_);
      #ifdef USE_SWITCH
      LOG_SWITCH("  ", "UpSwitch", this->up_switch_);
      LOG_SWITCH("  ", "DownSwitch", this->down_switch_);
      LOG_SWITCH("  ", "SetSwitch", this->set_switch_);
      LOG_SWITCH("  ", "MainSwitch", this->main_switch_);
      #endif
    }

    void VentAxiaSentinelKineticComponent::send_alive_str_() {
      this->calculate_command_(CMD_ALIVE_HEADER, CMD_ALIVE_DATA);
    }

    void VentAxiaSentinelKineticComponent::calculate_command_(const uint8_t * command_value, uint8_t command) {
      cmdbuffer_[0] = CMD_FRAME_HEADER;
      // command value bytes
      if (command_value != nullptr) {
        for (int i = 0; i < 4; i++) {
          cmdbuffer_[i + 1] = command_value[i];
        }
      }
      cmdbuffer_[5] = command;

      uint16_t crc = 0xFFFF;
      for (uint32_t i = 0; i < 6; i++) {
        crc -= cmdbuffer_[i];
      }
      cmdbuffer_[6] = highbyte(crc);
      cmdbuffer_[7] = lowbyte(crc);
      send_command_();
    }

    void VentAxiaSentinelKineticComponent::send_command_() {
      this->write_array(cmdbuffer_, 8);
    }

    bool VentAxiaSentinelKineticComponent::validate_crc_() {
      uint16_t crc = 0xFFFF;
      for (int i = 0; i < 39; i++) {
        crc -= buffer[i];
      }

      uint16_t received_crc = (buffer[39] << 8) | buffer[40];
      return (crc == received_crc);
    }

    void VentAxiaSentinelKineticComponent::set_down(bool enable) {
      CMD_KEY_DATA = (CMD_KEY_DATA & ~1) | enable;
    }
    void VentAxiaSentinelKineticComponent::set_up(bool enable) {
      CMD_KEY_DATA = (CMD_KEY_DATA & ~(1 << 1)) | (enable << 1);
    }
    void VentAxiaSentinelKineticComponent::set_set(bool enable) {
      CMD_KEY_DATA = (CMD_KEY_DATA & ~(1 << 2)) | (enable << 2);
    }
    void VentAxiaSentinelKineticComponent::set_main(bool enable) {
      CMD_KEY_DATA = (CMD_KEY_DATA & ~(1 << 3)) | (enable << 3);
    }

    void VentAxiaSentinelKineticComponent::publishDiagnosticIfValid(text_sensor::TextSensor* diag, const std::string& buff) {
      if (diag != nullptr) {
          diag->publish_state(buff.substr(18, 16).c_str());
      }
    }

    void VentAxiaSentinelKineticComponent::process_packet_() {
      if (std::memcmp(buffer, last_buffer, sizeof(buffer)) != 0){ //Only process the string if its changed
        std::string buff(reinterpret_cast < const char * > (buffer + 5), 34);
        this->line1_ -> publish_state(buff.substr(1, 16).c_str());
        this->line2_ -> publish_state(buff.substr(18, 16).c_str());

        if (buff.substr(1, 10) == "Diagnostic") {
          int code = std::stoi(buff.substr(13, 2)); // Convert to integer

          switch (code) {
          case 0: publishDiagnosticIfValid(this->diagnostic0_, buff); break;
          case 1: publishDiagnosticIfValid(this->diagnostic1_, buff); break;
          case 2: publishDiagnosticIfValid(this->diagnostic2_, buff); break;
          case 3: publishDiagnosticIfValid(this->diagnostic3_, buff); break;
          case 4: publishDiagnosticIfValid(this->diagnostic4_, buff); break;
          case 5: publishDiagnosticIfValid(this->diagnostic5_, buff); break;
          case 6: publishDiagnosticIfValid(this->diagnostic6_, buff); break;
          case 7: publishDiagnosticIfValid(this->diagnostic7_, buff); break;
          case 8: publishDiagnosticIfValid(this->diagnostic8_, buff); break;
          case 9: publishDiagnosticIfValid(this->diagnostic9_, buff); break;
          case 10: publishDiagnosticIfValid(this->diagnostic10_, buff); break;
          case 11: publishDiagnosticIfValid(this->diagnostic11_, buff); break;
          case 12: publishDiagnosticIfValid(this->diagnostic12_, buff); break;
          case 13: publishDiagnosticIfValid(this->diagnostic13_, buff); break;
          case 14: publishDiagnosticIfValid(this->diagnostic14_, buff); break;
          case 15: publishDiagnosticIfValid(this->diagnostic15_, buff); break;
          case 16: publishDiagnosticIfValid(this->diagnostic16_, buff); break;
          case 17: publishDiagnosticIfValid(this->diagnostic17_, buff); break;
          case 18: publishDiagnosticIfValid(this->diagnostic18_, buff); break;
          case 19: publishDiagnosticIfValid(this->diagnostic19_, buff); break;
          case 20: publishDiagnosticIfValid(this->diagnostic20_, buff); break;
          case 21: publishDiagnosticIfValid(this->diagnostic21_, buff); break;
          case 22: publishDiagnosticIfValid(this->diagnostic22_, buff); break;
          case 23: publishDiagnosticIfValid(this->diagnostic23_, buff); break;
          case 24: publishDiagnosticIfValid(this->diagnostic24_, buff); break;
          case 25: publishDiagnosticIfValid(this->diagnostic25_, buff); break;
          case 26: publishDiagnosticIfValid(this->diagnostic26_, buff); break;
          case 27: publishDiagnosticIfValid(this->diagnostic27_, buff); break;
          case 28: publishDiagnosticIfValid(this->diagnostic28_, buff); break;
          // case 0:
          //   if (this->diagnostic0_ != nullptr) {
          //     this->diagnostic0_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 1:
          //   if (this->diagnostic1_ != nullptr) {
          //     this->diagnostic1_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 2:
          //   if (this->diagnostic2_ != nullptr) {
          //     this->diagnostic2_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 3:
          //   if (this->diagnostic3_ != nullptr) {
          //     this->diagnostic3_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 4:
          //   if (this->diagnostic4_ != nullptr) {
          //     this->diagnostic4_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 5:
          //   if (this->diagnostic5_ != nullptr) {
          //     this->diagnostic5_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 6:
          //   if (this->diagnostic6_ != nullptr) {
          //     this->diagnostic6_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 7:
          //   if (this->diagnostic7_ != nullptr) {
          //     this->diagnostic7_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 8:
          //   if (this->diagnostic8_ != nullptr) {
          //     this->diagnostic8_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 9:
          //   if (this->diagnostic9_ != nullptr) {
          //     this->diagnostic9_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 10:
          //   if (this->diagnostic10_ != nullptr) {
          //     this->diagnostic10_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 11:
          //   if (this->diagnostic11_ != nullptr) {
          //     this->diagnostic11_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 12:
          //   if (this->diagnostic12_ != nullptr) {
          //     this->diagnostic12_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 13:
          //   if (this->diagnostic13_ != nullptr) {
          //     this->diagnostic13_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 14:
          //   if (this->diagnostic14_ != nullptr) {
          //     this->diagnostic14_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 15:
          //   if (this->diagnostic15_ != nullptr) {
          //     this->diagnostic15_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 16:
          //   if (this->diagnostic16_ != nullptr) {
          //     this->diagnostic16_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 17:
          //   if (this->diagnostic17_ != nullptr) {
          //     this->diagnostic17_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 18:
          //   if (this->diagnostic18_ != nullptr) {
          //     this->diagnostic18_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 19:
          //   if (this->diagnostic19_ != nullptr) {
          //     this->diagnostic19_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 20:
          //   if (this->diagnostic20_ != nullptr) {
          //     this->diagnostic20_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 21:
          //   if (this->diagnostic21_ != nullptr) {
          //     this->diagnostic21_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 22:
          //   if (this->diagnostic22_ != nullptr) {
          //     this->diagnostic22_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 23:
          //   if (this->diagnostic23_ != nullptr) {
          //     this->diagnostic23_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 24:
          //   if (this->diagnostic24_ != nullptr) {
          //     this->diagnostic24_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 25:
          //   if (this->diagnostic25_ != nullptr) {
          //     this->diagnostic25_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 26:
          //   if (this->diagnostic26_ != nullptr) {
          //     this->diagnostic26_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 27:
          //   if (this->diagnostic27_ != nullptr) {
          //     this->diagnostic27_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          // case 28:
          //   if (this->diagnostic28_ != nullptr) {
          //     this->diagnostic28_ -> publish_state(buff.substr(18, 16).c_str());
          //   }
          //   break;
          default:
            break;
          }
        }
        // last_buffer = buffer;
        std::memcpy(this->last_buffer, this->buffer, sizeof(buffer));
      }
    }

    void DiagnosticButton::dump_config() {
      LOG_BUTTON("", "Diagnostic Button", this);
    }

    void DiagnosticButton::press_action() {
      this->parent_ -> get_diagnostic = true;
    }

  } // namespace vent_axia_sentinel_kinetic
} // namespace esphome