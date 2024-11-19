#ifndef BGEIGIECAST_STATE_LED_H
#define BGEIGIECAST_STATE_LED_H

#include "rgb_led.h"
#include "local_storage.h"

#include <Supervisor.hpp>

/**
 * Controls the LED to visualize the current mode
 */
class ModeLED : private RGBLed, public Supervisor {
 public:
  typedef enum {
    mode_color_off,
    mode_color_init,
    mode_color_config,
    mode_color_mobile,
    mode_color_fixed_connected,
    mode_color_fixed_soft_error,
    mode_color_fixed_hard_error,
    mode_color_COUNT
  } ModeColor;

  explicit ModeLED(LocalStorage& config);
  virtual ~ModeLED() = default;

  /**
   * Set color
   * @param color : color type (ModeColor)
   * @param frequency : times per second to update if blink (0 for not blinking)
   *    - example, frequency of 1 will blink once a second
   * @param percentage_on : if frequency > 0, it will
   *    - example, percentage of 25 with 1 frequency, will 0.25 second LED on then 0.75 second LED off
   */
  void set_values(ModeColor color, double frequency = 0, uint8_t percentage_on = 50);
  void set_color(ModeColor color);

  void loop();

  void handle_report(const worker_map_t& workers, const handler_map_t& handlers) override;

  void initialize() override;

 private:
  typedef struct {
    RGB_e normal;
    RGB_e color_blind;
  } ColorType;

  uint8_t get_intensity() const override;

  const LocalStorage& _config;
  ModeColor color;
  double frequency;
  uint8_t percentage_on;
  const ColorType _colorTypes[ModeColor::mode_color_COUNT];
};

#endif //BGEIGIECAST_STATE_LED_H
