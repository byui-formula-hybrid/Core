#ifndef ARDUINO_ARCH_NATIVE

#include "hardware.h"

#include "../can/esp32_s3_can_service.h"
#include "../can/types.h"

#include "../../src/display_smoke.h"

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace {

// TODO: set to actual CAN pins for final hardware.
static constexpr CAN::PIN kCanTxPin = CAN::PIN::NUM_5;
static constexpr CAN::PIN kCanRxPin = CAN::PIN::NUM_4;

// TODO: confirm if dash owns this buzzer pin.
static constexpr gpio_num_t kRtdBuzzerPin = GPIO_NUM_6;

}  // namespace

namespace Dash {

Hardware::Hardware() : m_provider(nullptr), m_panel(nullptr) {
}

esp_err_t Hardware::init_can() {
    static CAN::ESP32S3CanService can_service;
    m_provider = std::make_shared<CAN::Provider>(&can_service, kCanTxPin, kCanRxPin);

    return m_provider->begin() ? ESP_OK : ESP_FAIL;
}

esp_err_t Hardware::init_display() {
    esp_err_t err = dash::display::init_smoke_display();
    if (err != ESP_OK) {
        return err;
    }

    m_panel = dash::display::get_panel_handle();
    return (m_panel == nullptr) ? ESP_FAIL : ESP_OK;
}

esp_err_t Hardware::init_touch() {
    // TODO: Integrate touch controller (GT911/FT5x06) and LVGL indev callbacks.
    return ESP_OK;
}

esp_err_t Hardware::buzz_rtd(uint32_t duration_ms, uint32_t freq_hz) {
    if (duration_ms < 1000U) {
        duration_ms = 1000U;
    }
    if (duration_ms > 3000U) {
        duration_ms = 3000U;
    }

    ledc_timer_config_t timer_cfg = {};
    timer_cfg.speed_mode = LEDC_LOW_SPEED_MODE;
    timer_cfg.duty_resolution = LEDC_TIMER_10_BIT;
    timer_cfg.timer_num = LEDC_TIMER_0;
    timer_cfg.freq_hz = static_cast<int>(freq_hz);
    timer_cfg.clk_cfg = LEDC_AUTO_CLK;

    if (ledc_timer_config(&timer_cfg) != ESP_OK) {
        return ESP_FAIL;
    }

    ledc_channel_config_t channel_cfg = {};
    channel_cfg.gpio_num = kRtdBuzzerPin;
    channel_cfg.speed_mode = LEDC_LOW_SPEED_MODE;
    channel_cfg.channel = LEDC_CHANNEL_0;
    channel_cfg.intr_type = LEDC_INTR_DISABLE;
    channel_cfg.timer_sel = LEDC_TIMER_0;
    channel_cfg.duty = 512;
    channel_cfg.hpoint = 0;

    if (ledc_channel_config(&channel_cfg) != ESP_OK) {
        return ESP_FAIL;
    }

    vTaskDelay(pdMS_TO_TICKS(duration_ms));

    if (ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0) != ESP_OK) {
        return ESP_FAIL;
    }
    if (ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}

}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
