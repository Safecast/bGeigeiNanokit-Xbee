#include "bgeigie_connector.h"
#include "debugger.h"

BGeigieConnector::BGeigieConnector(Stream& serial_connection) :
    Worker<Reading>(4000),
    _serial_connection(serial_connection),
    _buffer("") {
}

int8_t BGeigieConnector::produce_data() {
  while(_serial_connection.available() > 0) {
    char c = static_cast<char>(_serial_connection.read());
    _buffer += c;
    if(c == '\n') {
      data = _buffer.c_str();
      DEBUG_PRINTLN(_buffer);
      _buffer = "";
      // Clear rest of input buffer (should be empty after new measurement anyway)
      while (_serial_connection.available() > 0) _serial_connection.read();
      return data.get_status() & k_reading_parsed ? Worker::e_worker_data_read : Worker::e_worker_error;
    }
  }
  return Worker::e_worker_idle;
}
