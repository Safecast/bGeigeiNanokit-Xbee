#ifndef BGEIGIECAST_APICONNECTOR_H
#define BGEIGIECAST_APICONNECTOR_H

#include <WiFi.h>
#include <HTTPClient.h>

#include <Handler.hpp>

#include "local_storage.h"
#include "reading.h"
#include "user_config.h"
#include "circular_buffer.h"
#include "wifi_connection.h"

/**
 * Connects over WiFi to the API to send readings
 */
class ApiConnector : public Handler {
 public:
  /**
   * Collection of statuses for the api data handling, default is idle.
   */
  enum ApiHandlerStatus {
    e_api_reporter_send_success,
    e_api_reporter_error_to_json,
    e_api_reporter_error_not_connected,
    e_api_reporter_error_remote_not_available,
    e_api_reporter_error_server_rejected_post_400,
    e_api_reporter_error_server_rejected_post_401,
    e_api_reporter_error_server_rejected_post_403,
    e_api_reporter_error_server_rejected_post_5xx,
  };

  enum SendFrequency {
    e_api_send_frequency_5_sec,
    e_api_send_frequency_1_min,
    e_api_send_frequency_5_min,
  };

  explicit ApiConnector(LocalStorage& config);
  virtual ~ApiConnector() = default;

 protected:

  /**
   * Check if enough time has passed to send the latest reading to api
   * @param offset: additional ms offset (default 1 sec to overlap measurements better)
   * @return true if time to send
   */
  bool time_to_send(unsigned offset = 1000) const;

  /**
   * Initialize the connection
   * @param initial: set to false if its for reconnect / connect in error
   * @return true if connection with the wifi was made
   */
  bool activate(bool retry) override;

  /**
   * Stop the connection
   */
  void deactivate() override;

  int8_t handle_produced_work(const worker_map_t& workers) override;

  bool reading_to_json(const Reading& reading, char* out);

  /**
   * Send a reading to the API
   * @return: status
   */
  int8_t handle_async() override;

 private:

  LocalStorage& _config;
  char _payload[200];
  uint32_t _last_success_send;
};

#endif //BGEIGIECAST_APICONNECTOR_H
