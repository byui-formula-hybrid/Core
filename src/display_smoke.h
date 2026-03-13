#pragma once

#include "esp_err.h"

namespace dash {
namespace display {

// Initialize the 7-inch RGB panel and enable backlight.
esp_err_t init_smoke_display();

// Draw a full-screen RGB565 color bar test pattern.
esp_err_t draw_color_bars();

// Draw a full-screen solid RGB565 color.
esp_err_t draw_solid_color(uint16_t rgb565);

// Control the display backlight via onboard I2C expander.
esp_err_t set_backlight(bool on);

} // namespace display
} // namespace dash
