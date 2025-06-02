#include "hub.h"
#include "esphome/core/helpers.h"

#include <string>

namespace esphome {
namespace opentherm {

static const char *const TAG = "opentherm";
namespace message_data {

void write_flag8_lb_0(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 0, value); }
void write_flag8_lb_1(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 1, value); }
void write_flag8_lb_2(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 2, value); }
void write_flag8_lb_3(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 3, value); }
void write_flag8_lb_4(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 4, value); }
void write_flag8_lb_5(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 5, value); }
void write_flag8_lb_6(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 6, value); }
void write_flag8_lb_7(const bool value, OpenthermData &data) { data.valueLB = write_bit(data.valueLB, 7, value); }
void write_flag8_hb_0(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 0, value); }
void write_flag8_hb_1(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 1, value); }
void write_flag8_hb_2(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 2, value); }
void write_flag8_hb_3(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 3, value); }
void write_flag8_hb_4(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 4, value); }
void write_flag8_hb_5(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 5, value); }
void write_flag8_hb_6(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 6, value); }
void write_flag8_hb_7(const bool value, OpenthermData &data) { data.valueHB = write_bit(data.valueHB, 7, value); }
void write_u8_lb(const uint8_t value, OpenthermData &data) { data.valueLB = value; }
void write_u8_hb(const uint8_t value, OpenthermData &data) { data.valueHB = value; }
void write_s8_lb(const int8_t value, OpenthermData &data) { data.valueLB = (uint8_t) value; }
void write_s8_hb(const int8_t value, OpenthermData &data) { data.valueHB = (uint8_t) value; }
void write_u16(const uint16_t value, OpenthermData &data) { data.u16(value); }
void write_s16(const int16_t value, OpenthermData &data) { data.s16(value); }
void write_f88(const float value, OpenthermData &data) { data.f88(value); }

}  // namespace message_data

OpenthermData OpenthermHub::build_request_(MessageId request_id) const {
  OpenthermData data;
  data.type = 0;
  data.id = request_id;
  data.valueHB = 0;
  data.valueLB = 0;

  // We need this special logic for STATUS message because we have two options for specifying boiler modes:
  // with static config values in the hub, or with separate switches.
  if (request_id == MessageId::STATUS) {
    // NOLINTBEGIN
    bool const ch_enabled = this->ch_enable && OPENTHERM_READ_ch_enable && OPENTHERM_READ_t_set > 0.0;
    bool const dhw_enabled = this->dhw_enable && OPENTHERM_READ_dhw_enable;
    bool const cooling_enabled =
        this->cooling_enable && OPENTHERM_READ_cooling_enable && OPENTHERM_READ_cooling_control > 0.0;
    bool const otc_enabled = this->otc_active && OPENTHERM_READ_otc_active;
    bool const ch2_enabled = this->ch2_active && OPENTHERM_READ_ch2_active && OPENTHERM_READ_t_set_ch2 > 0.0;
    bool const summer_mode_is_active = this->summer_mode_active && OPENTHERM_READ_summer_mode_active;
    bool const dhw_blocked = this->dhw_block && OPENTHERM_READ_dhw_block;
    // NOLINTEND

    data.valueHB = ch_enabled | (dhw_enabled << 1) | (cooling_enabled << 2) | (otc_enabled << 3) | (ch2_enabled << 4) |
                   (summer_mode_is_active << 5) | (dhw_blocked << 6);

    return data;
  }

  // And if we get here, a message was requested which somehow wasn't handled.
  // This shouldn't happen due to the way the defines are configured, so we
  // log an error and just return a 0 message.
  ESP_LOGE(TAG, "Tried to create a request with unknown id %d. This should never happen, so please open an issue.",
           request_id);
  return {};
}

OpenthermHub::OpenthermHub() : Component(), in_pin_{}, out_pin_{} {}

void OpenthermHub::setup() {
  ESP_LOGD(TAG, "Setting up OpenTherm component");
  this->opentherm_ = make_unique<OpenTherm>(this->in_pin_, this->out_pin_);
  if (!this->opentherm_->initialize()) {
    ESP_LOGE(TAG, "Failed to initialize OpenTherm protocol. See previous log messages for details.");
    this->mark_failed();
    return;
  }

  // Ensure that there is at least one request, as we are required to
  // communicate at least once every second. Sending the status request is
  // good practice anyway.
  this->add_repeating_message(MessageId::STATUS);
  this->write_initial_messages_(this->messages_);
  this->message_iterator_ = this->messages_.begin();
}

void OpenthermHub::on_shutdown() { this->opentherm_->stop(); }

// Disabling clang-tidy for this particular line since it keeps removing the trailing underscore (bug?)
void OpenthermHub::write_initial_messages_(std::vector<MessageId> &target) {  // NOLINT
  std::vector<std::pair<MessageId, uint8_t>> sorted;
  std::copy_if(this->configured_messages_.begin(), this->configured_messages_.end(), std::back_inserter(sorted),
               [](const std::pair<MessageId, uint8_t> &pair) { return pair.second < REPEATING_MESSAGE_ORDER; });
  std::sort(sorted.begin(), sorted.end(),
            [](const std::pair<MessageId, uint8_t> &a, const std::pair<MessageId, uint8_t> &b) {
              return a.second < b.second;
            });

  target.clear();
  std::transform(sorted.begin(), sorted.end(), std::back_inserter(target),
                 [](const std::pair<MessageId, uint8_t> &pair) { return pair.first; });
}

// Disabling clang-tidy for this particular line since it keeps removing the trailing underscore (bug?)
void OpenthermHub::write_repeating_messages_(std::vector<MessageId> &target) {  // NOLINT
  target.clear();
  for (auto const &pair : this->configured_messages_) {
    if (pair.second == REPEATING_MESSAGE_ORDER) {
      target.push_back(pair.first);
    }
  }
}

void OpenthermHub::loop() {
  if (this->sync_mode_) {
    this->sync_loop_();
    return;
  }

  auto cur_time = millis();
  auto const cur_mode = this->opentherm_->get_mode();

  switch (cur_mode) {
    case OperationMode::WRITE:
    case OperationMode::IDLE:
      this->check_timings_(cur_time);
      if (this->should_skip_loop_(cur_time)) {
        break;
      }
      this->start_conversation_();
      break;
    case OperationMode::SENT:
      // Message sent, now listen for the response.
      break;
    default:
      break;
  }
  this->last_mode_ = cur_mode;
}

void OpenthermHub::sync_loop_() {
  if (!this->opentherm_->is_idle()) {
    ESP_LOGE(TAG, "OpenTherm is not idle at the start of the loop");
    return;
  }

  auto cur_time = millis();

  this->check_timings_(cur_time);

  if (this->should_skip_loop_(cur_time)) {
    return;
  }

  // Spin while message is being sent to device
  if (!this->spin_wait_(1150, [&] { return this->opentherm_->is_active(); })) {
    ESP_LOGE(TAG, "Hub timeout triggered during send");
    this->stop_opentherm_();
    return;
  }
}

void OpenthermHub::check_timings_(uint32_t cur_time) {
  if (this->last_conversation_start_ > 0 && (cur_time - this->last_conversation_start_) > 1150) {
    ESP_LOGW(TAG,
             "%d ms elapsed since the start of the last convo, but 1150 ms are allowed at maximum. Look at other "
             "components that might slow the loop down.",
             (int) (cur_time - this->last_conversation_start_));
  }
}

bool OpenthermHub::should_skip_loop_(uint32_t cur_time) const {
  if (this->last_conversation_end_ > 0 && (cur_time - this->last_conversation_end_) < 100) {
    ESP_LOGV(TAG, "Less than 100 ms elapsed since last convo, skipping this iteration");
    return true;
  }

  return false;
}

void OpenthermHub::start_conversation_() {
  if (this->message_iterator_ == this->messages_.end()) {
    if (this->sending_initial_) {
      this->sending_initial_ = false;
      this->write_repeating_messages_(this->messages_);
    }
    this->message_iterator_ = this->messages_.begin();
  }

  auto request = this->build_request_(*this->message_iterator_);

  this->before_send_callback_.call(request);

  ESP_LOGD(TAG, "Sending request with id %d (%s)", request.id,
           this->opentherm_->message_id_to_str((MessageId) request.id));
  this->opentherm_->debug_data(request);
  // Send the request
  this->last_conversation_start_ = millis();
  this->opentherm_->send(request);
}

void OpenthermHub::stop_opentherm_() {
  this->opentherm_->stop();
  this->last_conversation_end_ = millis();
}

void OpenthermHub::dump_config() {
  std::vector<MessageId> initial_messages;
  std::vector<MessageId> repeating_messages;
  this->write_initial_messages_(initial_messages);
  this->write_repeating_messages_(repeating_messages);

  ESP_LOGCONFIG(TAG, "OpenTherm:");
  LOG_PIN("  In: ", this->in_pin_);
  LOG_PIN("  Out: ", this->out_pin_);
  ESP_LOGCONFIG(TAG, "  Sync mode: %s", YESNO(this->sync_mode_));
  ESP_LOGCONFIG(TAG, "  Sensors: %s", SHOW(OPENTHERM_SENSOR_LIST(ID, )));
  ESP_LOGCONFIG(TAG, "  Binary sensors: %s", SHOW(OPENTHERM_BINARY_SENSOR_LIST(ID, )));
  ESP_LOGCONFIG(TAG, "  Switches: %s", SHOW(OPENTHERM_SWITCH_LIST(ID, )));
  ESP_LOGCONFIG(TAG, "  Input sensors: %s", SHOW(OPENTHERM_INPUT_SENSOR_LIST(ID, )));
  ESP_LOGCONFIG(TAG, "  Outputs: %s", SHOW(OPENTHERM_OUTPUT_LIST(ID, )));
  ESP_LOGCONFIG(TAG, "  Numbers: %s", SHOW(OPENTHERM_NUMBER_LIST(ID, )));
  ESP_LOGCONFIG(TAG, "  Initial requests:");
  for (auto type : initial_messages) {
    ESP_LOGCONFIG(TAG, "  - %d (%s)", type, this->opentherm_->message_id_to_str(type));
  }
  ESP_LOGCONFIG(TAG, "  Repeating requests:");
  for (auto type : repeating_messages) {
    ESP_LOGCONFIG(TAG, "  - %d (%s)", type, this->opentherm_->message_id_to_str(type));
  }
}

}  // namespace opentherm
}  // namespace esphome
