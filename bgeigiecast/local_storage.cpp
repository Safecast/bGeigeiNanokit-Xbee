
#include "local_storage.h"
#include "debugger.h"
#include "identifiers.h"
#include "reading.h"
#include "controller.h"
#include "bgeigie_connector.h"
#include "api_connector.h"

const char* memory_name = "data";

// Keys for config
const char* key_device_id = "ap_ssid";
const char* key_ap_password = "ap_password";
const char* key_wifi_ssid = "wifi_ssid";
const char* key_wifi_password = "wifi_password";
const char* key_api_key = "api_key";
const char* key_send_frequency = "send_frequency";
const char* key_led_color_blind = "led_color_blind";
const char* key_wifi_server = "wifi_server";
const char* key_led_color_intensity = "led_intensity";
const char* key_saved_state = "saved_state";
const char* key_home_longitude = "home_longtitude";
const char* key_home_latitude = "home_latitude";
const char* key_last_longitude = "last_longtitude";
const char* key_last_latitude = "last_latitude";

LocalStorage::LocalStorage() :
    Handler(),
    _memory(),
    _device_id(0),
    _ap_password(""),
    _wifi_ssid(""),
    _wifi_password(""),
    _api_key(""),
    _send_frequency(D_SEND_FREQUENCY),
    _led_color_blind(D_LED_COLOR_BLIND),
    _led_color_intensity(D_LED_COLOR_INTENSITY),
    _wifi_server(D_WIFI_SERVER),
    _saved_state(D_SAVED_STATE),
    _use_home_location(false),
    _home_longitude(0),
    _home_latitude(0),
    _last_longitude(0),
    _last_latitude(0) {
}

void LocalStorage::reset_defaults() {
  if(clear()) {
    set_device_id(D_DEVICE_ID, true);
    set_ap_password(D_ACCESS_POINT_PASSWORD, true);
    set_wifi_ssid(D_WIFI_SSID, true);
    set_wifi_password(D_WIFI_PASSWORD, true);
    set_api_key(D_APIKEY, true);
    set_send_frequency(D_SEND_FREQUENCY, true);
    set_led_color_blind(D_LED_COLOR_BLIND, true);
    set_led_color_intensity(D_LED_COLOR_INTENSITY, true);
    set_wifi_server(D_WIFI_SERVER, true);
    set_saved_state(D_SAVED_STATE, true);
    set_use_home_location(false, true);
    set_home_longitude(0, true);
    set_home_latitude(0, true);
    set_last_longitude(0, true);
    set_last_latitude(0, true);
  }
}

uint16_t LocalStorage::get_device_id() const {
  return _device_id;
}

const char* LocalStorage::get_ap_password() const {
  return _ap_password;
}

const char* LocalStorage::get_wifi_ssid() const {
  return _wifi_ssid;
}

const char* LocalStorage::get_wifi_password() const {
  return _wifi_password;
}

const char* LocalStorage::get_api_key() const {
  return _api_key;
}

int8_t LocalStorage::get_saved_state() const {
  return _saved_state;
}

bool LocalStorage::is_led_color_blind() const {
  return _led_color_blind;
}

uint8_t LocalStorage::get_led_color_intensity() const {
  return _led_color_intensity;
}

bool LocalStorage::get_wifi_server() const {
  return _wifi_server;
}

uint8_t LocalStorage::get_send_frequency() const {
  return _send_frequency;
}

bool LocalStorage::get_use_home_location() const {
  return _use_home_location;
}

double LocalStorage::get_home_longitude() const {
  return _home_longitude;
}

double LocalStorage::get_home_latitude() const {
  return _home_latitude;
}

double LocalStorage::get_last_longitude() const {
  return _last_longitude;
}

double LocalStorage::get_last_latitude() const {
  return _last_latitude;
}

void LocalStorage::set_device_id(uint16_t device_id, bool force) {
  if(force || (device_id != _device_id)) {
    if(_memory.begin(memory_name)) {
      _device_id = device_id;
      _memory.putUShort(key_device_id, _device_id);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for device_id");
    }
  }
}

void LocalStorage::set_ap_password(const char* ap_password, bool force) {
  if(force || (ap_password != nullptr && strlen(ap_password) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_ap_password, ap_password);
      _memory.putString(key_ap_password, _ap_password);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for ap_password");
    }
  }
}

void LocalStorage::set_wifi_ssid(const char* wifi_ssid, bool force) {
  if(force || (wifi_ssid != nullptr && strlen(wifi_ssid) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_wifi_ssid, wifi_ssid);
      _memory.putString(key_wifi_ssid, _wifi_ssid);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for wifi_ssid");
    }
  }
}

void LocalStorage::set_wifi_password(const char* wifi_password, bool force) {
  if(force || (wifi_password != nullptr && strlen(wifi_password) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_wifi_password, wifi_password);
      _memory.putString(key_wifi_password, _wifi_password);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for wifi_password");
    }
  }
}

void LocalStorage::set_api_key(const char* api_key, bool force) {
  if(force || (api_key != nullptr && strlen(api_key) < CONFIG_VAL_MAX)) {
    if(_memory.begin(memory_name)) {
      strcpy(_api_key, api_key);
      _memory.putString(key_api_key, _api_key);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for api_key");
    }
  }
}

void LocalStorage::set_send_frequency(uint8_t send_frequency, bool force) {
  if(force || (send_frequency != _send_frequency)) {
    if(_memory.begin(memory_name)) {
      _send_frequency = send_frequency;
      _memory.putUChar(key_send_frequency, send_frequency);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for key_send_frequency");
    }
  }
}

void LocalStorage::set_led_color_blind(bool led_color_blind, bool force) {
  if(force || (led_color_blind != _led_color_blind)) {
    if(_memory.begin(memory_name)) {
      _led_color_blind = led_color_blind;
      _memory.putBool(key_led_color_blind, led_color_blind);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for key_led_color_blind");
    }
  }
}

void LocalStorage::set_led_color_intensity(uint8_t led_color_intensity, bool force) {
  if(force || (led_color_intensity != _led_color_intensity)) {
    if(_memory.begin(memory_name)) {
      _led_color_intensity = led_color_intensity;
      _memory.putUChar(key_led_color_intensity, led_color_intensity);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for key_led_color_intensity");
    }
  }
}

void LocalStorage::set_wifi_server(bool wifi_server, bool force) {
  if(force || (wifi_server != _wifi_server)) {
    if(_memory.begin(memory_name)) {
      _wifi_server = wifi_server;
      _memory.putBool(key_wifi_server, wifi_server);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for key_wifi_server");
    }
  }
}

void LocalStorage::set_saved_state(uint8_t saved_state, bool force) {
  if(force || (saved_state != _saved_state)) {
    if(_memory.begin(memory_name)) {
      _saved_state = saved_state;
      _memory.putChar(key_saved_state, saved_state);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for init_fixed");
    }
  }
}

void LocalStorage::set_use_home_location(bool use_home_location, bool force) {
  if(force || (use_home_location != _use_home_location)) {
    if(_memory.begin(memory_name)) {
      _use_home_location = use_home_location;
      _memory.putBool(key_led_color_blind, use_home_location);
      _memory.end();
    } else {
      DEBUG_PRINTLN("unable to save new value for key_led_color_blind");
    }
  }
}

void LocalStorage::set_home_longitude(double home_longtitude, bool force) {
  if(_memory.begin(memory_name)) {
    _home_longitude = home_longtitude;
    _memory.putDouble(key_home_longitude, home_longtitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_home_longitude");
  }
}

void LocalStorage::set_home_latitude(double home_latitude, bool force) {
  if(_memory.begin(memory_name)) {
    _home_latitude = home_latitude;
    _memory.putDouble(key_home_latitude, home_latitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_home_latitude");
  }
}

void LocalStorage::set_last_longitude(double last_longitude, bool force) {
  if(_memory.begin(memory_name)) {
    _last_longitude = last_longitude;
    _memory.putDouble(key_last_longitude, last_longitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_last_longitude");
  }
}

void LocalStorage::set_last_latitude(double last_latitude, bool force) {
  if(_memory.begin(memory_name)) {
    _last_latitude = last_latitude;
    _memory.putDouble(key_last_latitude, last_latitude);
    _memory.end();
  } else {
    DEBUG_PRINTLN("unable to save new value for key_last_latitude");
  }
}

bool LocalStorage::clear() {
  if(_memory.begin(memory_name)) {
    _memory.clear();
    _memory.end();
    return true;
  }
  return false;
}

bool LocalStorage::activate(bool) {
  _memory.begin(memory_name, false);
  _device_id = _memory.getUShort(key_device_id, D_DEVICE_ID);
  if(_memory.getString(key_ap_password, _ap_password, CONFIG_VAL_MAX) == 0) {
    strcpy(_ap_password, D_ACCESS_POINT_PASSWORD);
  }
  if(_memory.getString(key_wifi_ssid, _wifi_ssid, CONFIG_VAL_MAX) == 0) {
    strcpy(_wifi_ssid, D_WIFI_SSID);
  }
  if(_memory.getString(key_wifi_password, _wifi_password, CONFIG_VAL_MAX) == 0) {
    strcpy(_wifi_password, D_WIFI_PASSWORD);
  }
  if(_memory.getString(key_api_key, _api_key, CONFIG_VAL_MAX) == 0) {
    strcpy(_api_key, D_APIKEY);
  }
  _send_frequency = _memory.getUChar(key_send_frequency, D_SEND_FREQUENCY);
  _led_color_blind = _memory.getBool(key_led_color_blind, D_LED_COLOR_BLIND);
  _led_color_intensity = _memory.getUChar(key_led_color_intensity, D_LED_COLOR_INTENSITY);
  _saved_state = _memory.getChar(key_saved_state, D_SAVED_STATE);
  _use_home_location = _memory.getBool(key_led_color_blind, false);
  _home_longitude = _memory.getDouble(key_home_longitude, 0);
  _home_latitude = _memory.getDouble(key_home_latitude, 0);
  _last_longitude = _memory.getDouble(key_last_longitude, 0);
  _last_latitude = _memory.getDouble(key_last_latitude, 0);
  _memory.end();
  return true;
}

int8_t LocalStorage::handle_produced_work(const worker_map_t& workers) {
  // Get reading data to store
  const auto& reader = (BGeigieConnector*) workers.at(k_worker_bgeigie_connector);
  if(reader->is_fresh()) {
    const auto& reading = reader->get_data();
    set_device_id(reading.get_device_id(), false);
    if(reading.get_status() & k_reading_gps_ok) {
      set_last_latitude(reading.get_latitude(), false);
      set_last_longitude(reading.get_longitude(), false);
    }
    return Handler::e_handler_data_handled;
  }
  return Handler::e_handler_idle;
}
