#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#define BGEIGIECAST_VERSION "1.5"

/** System config **/
#define DEBUG_BAUD 115200
#define DEBUG_FULL_REPORT 0
#define BGEIGIE_CONNECTION_BAUD 9600
#define POST_INITIALIZE_DURATION 4000

/** Hardware pins settings **/
#ifndef MODE_BUTTON_PIN
#define MODE_BUTTON_PIN 0u
#endif

#ifndef USE_FASTLED
// 3-channel RGB LED (cathode/anode)
#define RGB_LED_PIN_R A18
#define RGB_LED_PIN_G A4
#define RGB_LED_PIN_B A5
#define CHANNEL_R 0
#define CHANNEL_G 1
#define CHANNEL_B 2
#define CHANNEL_FREQUENCY 12800
#define CHANNEL_RESOLUTION 8
#endif //USE_FASTLED


/** API connector settings **/
#define API_HOST "tt.safecast.org"
#define HEADER_API_CONTENT_TYPE "application/json"
#define HEADER_API_USER_AGENT "bGeigieCast/" BGEIGIECAST_VERSION
#define API_MEASUREMENTS_ENDPOINT "http://" API_HOST "/measurements.json"
#define API_READING_BUFFER 60 //

/** Access point settings **/
#define ACCESS_POINT_SSID       "bgeigie%d" // With device id
#define SERVER_WIFI_PORT        80
#define ACCESS_POINT_IP         {192, 168, 5, 1}
#define ACCESS_POINT_NMASK      {255, 255, 255, 0}

/** Default ESP configurations **/
#define D_DEVICE_ID             0
#define D_ACCESS_POINT_PASSWORD "safecast"
#define D_WIFI_SSID             "your_wifi_ssid"
#define D_WIFI_PASSWORD         "yourwifipassword"
#define D_APIKEY                ""
#define D_LED_COLOR_BLIND       false
#define D_LED_COLOR_INTENSITY   30
#define D_WIFI_SERVER           false
#define D_SAVED_STATE           Controller::k_savable_MobileMode
#define D_SEND_FREQUENCY        ApiConnector::e_api_send_frequency_5_min

#endif