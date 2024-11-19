
#include "api_connector.h"
#include "debugger.h"
#include "identifiers.h"
#include "bgeigie_connector.h"

#define RETRY_TIMEOUT 10000
#define HOME_LOCATION_PRECISION_KM 0.4

// subtracting 1 seconds so data is sent more often than not.
#define SEND_FREQUENCY(last_send, sec, slack) (last_send == 0 || (millis() - last_send) > ((sec * 1000) - slack))

ApiConnector::ApiConnector(LocalStorage& config) :
    Handler(),
    _config(config),
    _payload("") {
}

bool ApiConnector::time_to_send(unsigned offset) const {
  switch(static_cast<SendFrequency>(_config.get_send_frequency())) {
    case e_api_send_frequency_5_sec:
      return SEND_FREQUENCY(_last_success_send, 5, offset);
    case e_api_send_frequency_1_min:
      return SEND_FREQUENCY(_last_success_send, 60, offset);
    case e_api_send_frequency_5_min:
    default:  // Default 5 min frequency
      return SEND_FREQUENCY(_last_success_send, 5 * 60, offset);
  }
}

bool ApiConnector::activate(bool retry) {
  static uint32_t last_try = 0;
  if(WiFiConnection::wifi_connected()) {
    return true;
  }
  if(retry && millis() - last_try < RETRY_TIMEOUT) {
    return false;
  }
  last_try = millis();

  WiFiConnection::connect_wifi(_config.get_wifi_ssid(), _config.get_wifi_password());

  return WiFi.isConnected();
}

void ApiConnector::deactivate() {
  WiFiConnection::disconnect_wifi();
}

int8_t ApiConnector::handle_produced_work(const worker_map_t& workers) {
  const auto& geigie_connector = workers.worker<BGeigieConnector>(k_worker_bgeigie_connector);

  if(!geigie_connector || !geigie_connector->is_fresh()) {
    // No fresh data
    return e_handler_idle;
  }

  if(!time_to_send()) {
    if (time_to_send(6000)) {
      // almost time to send, start Wi-Fi if not connected yet
      activate(true);
    }
    return e_handler_idle;
  }

  const auto& reading = geigie_connector->get_data();

  if(!reading.valid_reading()) {
    return e_handler_idle;
  }

  if (_config.get_use_home_location() && !reading
      .near_coordinates(_config.get_home_latitude(), _config.get_home_longitude(), HOME_LOCATION_PRECISION_KM)) {
    // Reading not near home location
    DEBUG_PRINTLN("Reading not near configured home location");
    return e_handler_idle;
  }

  // All checks good, prep send data

  if(!reading_to_json(reading, _payload)) {
    DEBUG_PRINTLN("Unable to send, reading to payload conversion error");
    return e_api_reporter_error_to_json;
  }

  if(!WiFi.isConnected() && !activate(true)) {
    DEBUG_PRINTLN("Unable to send, lost connection");
    return e_api_reporter_error_not_connected;
  }

  return start_task("api_send", 2048 * 4, 3, 0);
}


int8_t ApiConnector::handle_async() {

  HTTPClient http;

  char url[100];
  sprintf(url, "%s?api_key=%s", API_MEASUREMENTS_ENDPOINT, _config.get_api_key());

  //Specify destination for HTTP request
  if(!http.begin(url)) {
    DEBUG_PRINTLN("Unable to begin url connection");
    http.end();  //Free resources
    return e_api_reporter_error_remote_not_available;
  }


  http.setUserAgent(HEADER_API_USER_AGENT);
  http.addHeader("Host", API_HOST);
  http.addHeader("Content-Type", HEADER_API_CONTENT_TYPE);

  int httpResponseCode = http.POST(_payload);

  auto now = millis();

  String response = http.getString();
  DEBUG_PRINTF("POST complete, status %d\r\nresponse: \r\n\r\n%s\r\n\r\n", httpResponseCode, response.c_str());
  http.end();  //Free resources

  if (!_config.get_wifi_server() && _config.get_send_frequency() != e_api_send_frequency_5_sec) {
    // Disconnect from Wi-Fi between readings (not needed when sending every 5 seconds)
    WiFiConnection::disconnect_wifi();
  }

  switch(httpResponseCode) {
    case 200 ... 204:
      _last_success_send = now;
      return e_api_reporter_send_success;
    case 400:
      return e_api_reporter_error_server_rejected_post_400;
    case 401:
      return e_api_reporter_error_server_rejected_post_401;
    case 403:
      return e_api_reporter_error_server_rejected_post_403;
    case 500 ... 599:
      return e_api_reporter_error_server_rejected_post_5xx;
    default:
      return e_api_reporter_error_remote_not_available;
  }

}

bool ApiConnector::reading_to_json(const Reading& reading, char* out) {
  if(!reading.valid_reading()) {
    return false;
  }

  sprintf(
      out,
      "{\"captured_at\":\"%s\","
      "\"device_id\":%d,"
      "\"value\":%d,"
      "\"unit\":\"cpm\","
      "\"longitude\":%.5f,"
      "\"latitude\":%.5f}\n",
      reading.get_iso_timestamp(),
      reading.get_fixed_device_id(),
      reading.get_cpm(),
      _config.get_use_home_location() ? _config.get_home_longitude() : reading.get_longitude(),
      _config.get_use_home_location() ? _config.get_home_latitude() : reading.get_latitude()
  );
  return true;
}


