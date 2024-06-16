
#include "mode_led.h"
#include "user_config.h"
#include "debugger.h"
#include "identifiers.h"
#include "sm_c_state.h"
#include "bluetooth_reporter.h"
#include "api_connector.h"
#include "bgeigie_connector.h"

// Set this to true if we use anode LED
#define RGB_STATE_LED_REVERSED true

ModeLED::ModeLED(LocalStorage& config) :
#ifdef USE_FASTLED
    RGBLed(),
#else
    RGBLed(RGB_LED_PIN_R, RGB_LED_PIN_G, RGB_LED_PIN_B, RGB_STATE_LED_REVERSED),
#endif //USE_FASTLED
    Supervisor(),
    _config(config),
    color(ModeColor::mode_color_off),
    frequency(0),
    percentage_on(0),
    _colorTypes{
        // Normal            Colorblind
        // R    G    B   ,   R    G    B
        {{000, 000, 000}, {000, 000, 000}}, // off
        {{063, 127, 063}, {063, 127, 063}}, // init
        {{127, 000, 127}, {127, 000, 127}}, // config
        {{000, 100, 255}, {000, 100, 255}}, // mobile
        {{000, 255, 000}, {000, 255, 000}}, // fixed_connected
        {{127, 127, 000}, {127, 127, 000}}, // fixed_connection_error
        {{255, 000, 000}, {255, 000, 000}}, // nano_error
    } {
}

void ModeLED::set_values(ModeLED::ModeColor _color, double _frequency, uint8_t _percentage_on) {
  frequency = _frequency;
  percentage_on = _percentage_on;
  color = _color;
  if(frequency == 0)
    set_color(color);
}

void ModeLED::set_color(ModeLED::ModeColor _color) {
  static ModeColor _last = mode_color_off;
  if(_color != _last) {
    _last = _color;
//    DEBUG_PRINTF("Changed LED to %d\n", _color);
    set(_config.is_led_color_blind() ? _colorTypes[_color].color_blind : _colorTypes[_color].normal);
  }
}

void ModeLED::loop() {
  if(frequency > 0) {
    // Blink LED
    double blink_millis = 1000 / frequency;
    auto cycle_now = millis() % static_cast<uint32_t>(blink_millis);
    auto threshold = static_cast<uint32_t>(blink_millis * (percentage_on / 100.0));
    if(cycle_now < threshold) {
      set_color(color);
    } else if(cycle_now > threshold) {
      set_color(ModeLED::mode_color_off);
    }
  }
}

uint8_t ModeLED::get_intensity() const {
  return _config.get_led_color_intensity();
}

void ModeLED::initialize() {
  init();
}

void ModeLED::handle_report(const worker_map_t& workers, const handler_map_t& handlers) {
  const auto& controller = workers.worker<Controller>(k_worker_controller_state);
  // Switch controller system state
  switch(controller->get_data()) {
    case SystemStateId::k_state_InitializeState:
    case SystemStateId::k_state_InitReadingState:
      /// Initializing, blink every 1 second
      set_values(mode_color_init, 1, 10);
      return;
    case SystemStateId::k_state_PostInitializeState:
      /// Post initializing, blink every 0.8 second ( this lasts for 3 seconds total )
      set_values(mode_color_init, 1.5, 15);
      return;
    case SystemStateId::k_state_ConfigurationModeState:
      switch(workers.at(k_worker_configuration_server)->get_active_state()) {
        case BaseWorker::e_state_active:
          /// Configuration mode - up and running. no blink
          set_values(mode_color_config);
          return;
        case BaseWorker::e_state_activating_failed:
        default:
          /// Configuration mode - connecting to wifi / settings up access point. blink
          set_values(mode_color_config, 1, 50);
          return;
      }
      return;
    case SystemStateId::k_state_MobileModeState:
      switch(handlers.at(k_handler_bluetooth_reporter)->get_status()) {
        case BluetoothReporter::e_handler_clients_available:
          /// Mobile mode - Clients connected
          set_values(mode_color_mobile);
          return;
        case BluetoothReporter::e_handler_idle:
        case BluetoothReporter::e_handler_no_clients:
        default:
          /// Mobile mode - No clients connected
          set_values(mode_color_mobile, 0.33, 5);
          return;
      }
      return;
    case SystemStateId::k_state_FixedModeState: {
      if(handlers.at(k_handler_api_reporter)->get_active_state() == Handler::e_state_activating_failed) {
        /// Fixed mode - Connecting to wifi, blink connected
        set_values(mode_color_fixed_connected, 1, 25);
        return;
      }
      const auto readingStatus = workers.worker<BGeigieConnector>(k_worker_bgeigie_connector)->get_data().get_status();
      if (readingStatus & k_reading_parsed && !(readingStatus & k_reading_sensor_ok)) {
        /// Fixed mode - Sensor broken
        set_values(mode_color_fixed_hard_error);
        return;
      }
      if (readingStatus & k_reading_parsed && !(readingStatus & k_reading_gps_ok)) {
        /// Fixed mode - GPS not available
        set_values(mode_color_fixed_hard_error, 0.5, 25);
        return;
      }
      switch(handlers.at(k_handler_api_reporter)->get_status()) {
        case ApiConnector::e_handler_processing:
          /// Fixed mode - Async sending to api, blink furiously
          set_values(mode_color_fixed_connected, 5);
          return;
        case ApiConnector::e_api_reporter_error_not_connected:
          /// Fixed mode - Lost connection to wifi, blink connected again
          set_values(mode_color_fixed_connected, 1, 25);
          return;
        case ApiConnector::e_api_reporter_error_remote_not_available:
        case ApiConnector::e_api_reporter_error_server_rejected_post_400:
        case ApiConnector::e_api_reporter_error_server_rejected_post_401:
        case ApiConnector::e_api_reporter_error_server_rejected_post_403:
        case ApiConnector::e_api_reporter_error_server_rejected_post_5xx:
          /// Fixed mode - Remote is not available
          set_values(mode_color_fixed_soft_error);
          return;
        case ApiConnector::e_api_reporter_send_success:
          /// Fixed mode - All good and connected
          set_values(mode_color_fixed_connected);
          return;
        default:
          return;
      }
    }
    case SystemStateId::k_state_ResetState:
      /// Reset - Display red because its least used
      set_values(mode_color_fixed_hard_error);
      return;
    default:
      return;
  }
}
