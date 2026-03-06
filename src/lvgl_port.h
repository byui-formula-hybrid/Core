#ifndef ARDUINO_ARCH_NATIVE

#pragma once

#include "esp_err.h"
#include "esp_lcd_types.h"
#include "esp_lcd_touch.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize LVGL with the display and touch controller
 * 
 * @param lcd_handle LCD panel handle
 * @param tp_handle Touch panel handle
 * @return ESP_OK on success
 */
esp_err_t lvgl_port_init(esp_lcd_panel_handle_t lcd_handle, esp_lcd_touch_handle_t tp_handle);

/**
 * @brief Take LVGL mutex (for thread-safe LVGL API calls)
 * 
 * @param timeout_ms Timeout in milliseconds (0 = wait forever)
 * @return true if mutex was taken, false if timeout
 */
bool lvgl_port_lock(int timeout_ms);

/**
 * @brief Release LVGL mutex
 */
void lvgl_port_unlock(void);

/**
 * @brief Notify LVGL that RGB vsync occurred (called from ISR)
 * @return true if higher priority task was woken
 */
bool lvgl_port_notify_rgb_vsync(void);

#ifdef __cplusplus
}
#endif

#endif // ARDUINO_ARCH_NATIVE
