#include "esphome/core/log.h"

#include "vent_axia_sentinel_kinetic.h"

#define highbyte(val)(uint8_t)((val) >> 8)
#define lowbyte(val)(uint8_t)((val) & 0xff)

namespace esphome {
  namespace vent_axia_sentinel_kinetic {

    static
    const char * TAG = "vent_axia_sentinel_kinetic.component";

    //ESP32
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
    //ESP32^^

    void VentAxiaSentinelKineticComponent::setup() {
      ESP_LOGCONFIG(TAG, "Setting up VentAxiaSentinelKinetic...");
      //ESP32
      instance = this;
      timer = timerBegin(1, 80, true);
      timerAttachInterrupt(timer, &timer_isr_wrapper, true);
      timerAlarmWrite(timer, 26000, true);  // 1 second interval
      // timerAlarmEnable(timer);
      //ESP32^^
      this -> send_alive_str_();
      ESP_LOGCONFIG(TAG, "VentAxiaSentinelKinetic setup complete.");
    }

    void VentAxiaSentinelKineticComponent::loop() {
      //Send serial packets
      if (CMD_KEY_DATA != 0) {
        if (CMD_KEY_DATA != LAST_CMD_KEY_DATA_) {
          this -> calculate_command_(CMD_KEY_HEADER, CMD_KEY_DATA);
          LAST_CMD_KEY_DATA_ = CMD_KEY_DATA;
        }
        timerAlarmEnable(timer);
        // // //Send serial packets
        // int32_t now = millis();
        // if (now - last_periodic_millis_ >= 20) { //we need every 26ms, but setting less can allow for more jitter
        //   last_periodic_millis_ = now;
        //   send_command_();
        // }
      } else {
        if (CMD_KEY_DATA != LAST_CMD_KEY_DATA_) {
          timerAlarmDisable(timer);
        }
      }

      //Recieve Serial packets
      if (this -> available() != 0) {
        uint8_t c;
        this -> read_byte( & c);

        if (current_index_ == 0 && c != 0x02) {
          return; // Wait for header
        }

        buffer[current_index_++] = c;

        if (current_index_ == sizeof(buffer)) {
          if (validate_crc_()) {
            packet_ready = true;
            process_packet_();
          }
          current_index_ = 0;
        }
      }
    }

    void VentAxiaSentinelKineticComponent::dump_config() {
      this -> check_uart_settings(9600, 1, esphome::uart::UART_CONFIG_PARITY_NONE, 8);
      ESP_LOGCONFIG(TAG, "Vent Axia Sentinel Kinetic Component");
      if (this -> is_failed()) {
        ESP_LOGE(TAG, "Connection with VentAxiaSentinelKinetic failed!");
      }
      LOG_TEXT_SENSOR("", "Line1 Sensor", this -> line1_);
      LOG_TEXT_SENSOR("", "Line2 Sensor", this -> line2_);
      #ifdef USE_SWITCH
      LOG_SWITCH("  ", "UpSwitch", this -> up_switch_);
      LOG_SWITCH("  ", "DownSwitch", this -> down_switch_);
      LOG_SWITCH("  ", "SetSwitch", this -> set_switch_);
      LOG_SWITCH("  ", "MainSwitch", this -> main_switch_);
      #endif
    }

    void VentAxiaSentinelKineticComponent::send_alive_str_() {
      this -> calculate_command_(CMD_ALIVE_HEADER, CMD_ALIVE_DATA);
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
      this -> write_array(cmdbuffer_, 8);
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

    void VentAxiaSentinelKineticComponent::process_packet_() {
      std::string buff(reinterpret_cast <
        const char * > (buffer + 5), 34);

      this -> line1_ -> publish_state(buff.substr(1, 16).c_str());
      this -> line2_ -> publish_state(buff.substr(18, 16).c_str());

      if (buff.substr(1, 10) == "Diagnostic") {
        int code = std::stoi(buff.substr(13, 2)); // Convert to integer

        switch (code) {
        case 0:
          if (this -> diagnostic0_ != nullptr) {
            this -> diagnostic0_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 1:
          if (this -> diagnostic1_ != nullptr) {
            this -> diagnostic1_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 2:
          if (this -> diagnostic2_ != nullptr) {
            this -> diagnostic2_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 3:
          if (this -> diagnostic3_ != nullptr) {
            this -> diagnostic3_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 4:
          if (this -> diagnostic4_ != nullptr) {
            this -> diagnostic4_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 5:
          if (this -> diagnostic5_ != nullptr) {
            this -> diagnostic5_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 6:
          if (this -> diagnostic6_ != nullptr) {
            this -> diagnostic6_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 7:
          if (this -> diagnostic7_ != nullptr) {
            this -> diagnostic7_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 8:
          if (this -> diagnostic8_ != nullptr) {
            this -> diagnostic8_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 9:
          if (this -> diagnostic9_ != nullptr) {
            this -> diagnostic9_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 10:
          if (this -> diagnostic10_ != nullptr) {
            this -> diagnostic10_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 11:
          if (this -> diagnostic11_ != nullptr) {
            this -> diagnostic11_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 12:
          if (this -> diagnostic12_ != nullptr) {
            this -> diagnostic12_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 13:
          if (this -> diagnostic13_ != nullptr) {
            this -> diagnostic13_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 14:
          if (this -> diagnostic14_ != nullptr) {
            this -> diagnostic14_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 15:
          if (this -> diagnostic15_ != nullptr) {
            this -> diagnostic15_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 16:
          if (this -> diagnostic16_ != nullptr) {
            this -> diagnostic16_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 17:
          if (this -> diagnostic17_ != nullptr) {
            this -> diagnostic17_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 18:
          if (this -> diagnostic18_ != nullptr) {
            this -> diagnostic18_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 19:
          if (this -> diagnostic19_ != nullptr) {
            this -> diagnostic19_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 20:
          if (this -> diagnostic20_ != nullptr) {
            this -> diagnostic20_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 21:
          if (this -> diagnostic21_ != nullptr) {
            this -> diagnostic21_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 22:
          if (this -> diagnostic22_ != nullptr) {
            this -> diagnostic22_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 23:
          if (this -> diagnostic23_ != nullptr) {
            this -> diagnostic23_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 24:
          if (this -> diagnostic24_ != nullptr) {
            this -> diagnostic24_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 25:
          if (this -> diagnostic25_ != nullptr) {
            this -> diagnostic25_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 26:
          if (this -> diagnostic26_ != nullptr) {
            this -> diagnostic26_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 27:
          if (this -> diagnostic27_ != nullptr) {
            this -> diagnostic27_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        case 28:
          if (this -> diagnostic28_ != nullptr) {
            this -> diagnostic28_ -> publish_state(buff.substr(18, 16).c_str());
          }
          break;
        default:
          break;
        }
      }
    }

  } // namespace vent_axia_sentinel_kinetic
} // namespace esphome