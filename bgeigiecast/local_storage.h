#ifndef BGEIGIECAST_ESP_CONFIG_H
#define BGEIGIECAST_ESP_CONFIG_H

#include <Preferences.h>

#include <Handler.hpp>

#define CONFIG_VAL_MAX 32

/**
 * Configurations for the ESP32, stored in the flash memory
 */
class LocalStorage : public Handler {
 public:
  LocalStorage();
  virtual ~LocalStorage() = default;

  /**
   * Reset settings to default (defined in user_config)
   */
  void reset_defaults();

  // Getters and setters
  virtual uint16_t get_device_id() const final;
  virtual const char* get_ap_password() const final;
  virtual const char* get_wifi_ssid() const final;
  virtual const char* get_wifi_password() const final;
  virtual const char* get_api_key() const final;
  virtual uint8_t get_send_frequency() const final;
  virtual bool is_led_color_blind() const final;
  virtual uint8_t get_led_color_intensity() const final;
  virtual bool get_wifi_server() const final;
  virtual int8_t get_saved_state() const final;
  virtual bool get_use_home_location() const final;
  virtual double get_home_longitude() const final;
  virtual double get_home_latitude() const final;
  virtual double get_last_longitude() const final;
  virtual double get_last_latitude() const final;

  virtual void set_device_id(uint16_t device_id, bool force);
  virtual void set_ap_password(const char* ap_password, bool force);
  virtual void set_wifi_ssid(const char* wifi_ssid, bool force);
  virtual void set_wifi_password(const char* wifi_password, bool force);
  virtual void set_api_key(const char* api_key, bool force);
  virtual void set_send_frequency(uint8_t send_frequency, bool force);
  virtual void set_led_color_blind(bool led_color_blind, bool force);
  virtual void set_led_color_intensity(uint8_t led_color_intensity, bool force);
  virtual void set_wifi_server(bool wifi_server, bool force);
  virtual void set_saved_state(uint8_t saved_state, bool force);
  virtual void set_use_home_location(bool use_home_location, bool force);
  virtual void set_home_longitude(double home_longitude, bool force);
  virtual void set_home_latitude(double home_latitude, bool force);
  virtual void set_last_longitude(double last_longitude, bool force);
  virtual void set_last_latitude(double last_latitude, bool force);

 protected:
  virtual bool clear();

  /**
   * Read all settings
   */
  bool activate(bool) override;
  int8_t handle_produced_work(const worker_map_t& workers) override;
 private:
  Preferences _memory;

  // Device
  uint16_t _device_id;

  // Access point config (for web _ap_server)
  char _ap_password[CONFIG_VAL_MAX];

  // Wifi config (to connect to the internet)
  char _wifi_ssid[CONFIG_VAL_MAX];
  char _wifi_password[CONFIG_VAL_MAX];

  // API config (to connect to the API)
  char _api_key[CONFIG_VAL_MAX];
  uint8_t _send_frequency;

  // RGB LED config
  bool _led_color_blind;
  uint8_t _led_color_intensity;

  bool _wifi_server;

  // Flag if fixed or mobile mode is started after init
  int8_t _saved_state;

  // Home location configs
  bool _use_home_location;
  double _home_longitude;
  double _home_latitude;

  double _last_longitude;
  double _last_latitude;
};

#endif //BGEIGIECAST_ESP_CONFIG_H
