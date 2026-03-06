#ifndef ARDUINO_ARCH_NATIVE

#pragma once

#include "esp_err.h"
#include "esp_lcd_types.h"
#include "esp_lcd_touch.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the Waveshare ESP32-S3-Touch-LCD-7 display
 * 
 * Sets up the 800×480 RGB parallel interface display with GT911 touch controller
 * 
 * @return ESP_OK on success
 */
esp_err_t display_7inch_init(void);

/**
 * @brief Get the LCD panel handle
 * @return Panel handle for use with LVGL
 */
esp_lcd_panel_handle_t display_7inch_get_panel(void);

/**
 * @brief Get the touch controller handle
 * @return Touch handle for use with LVGL
 */
esp_lcd_touch_handle_t display_7inch_get_touch(void);

/**
 * @brief Turn on the display backlight
 * @return ESP_OK on success
 */
esp_err_t display_7inch_backlight_on(void);

/**
 * @brief Turn off the display backlight
 * @return ESP_OK on success
 */
esp_err_t display_7inch_backlight_off(void);

#ifdef __cplusplus
}
#endif

#endif // ARDUINO_ARCH_NATIVE
