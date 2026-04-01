#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ui.h"
#include "controller.h"
#include "data_model.h"
#include "hardware.h"
#include "free_rtos_lock_strategy.h"
#include "free_rtos_thread_strategy.h"

static const char *TAG = "main";

extern "C" void app_main() {
    ESP_LOGI(TAG, "Dash UI + Hardware boot");

    Dash::Hardware hw;

    if (hw.init_display() != ESP_OK) {
        ESP_LOGE(TAG, "display init failed");
        return;
    }

    bool can_ready = (hw.init_can() == ESP_OK);
    if (!can_ready) {
        ESP_LOGW(TAG, "can init failed; continuing with display-only mode");
    }

    if (hw.init_touch() != ESP_OK) {
        ESP_LOGW(TAG, "touch init failed; continuing without touch input");
    }

    Dash::UI::UI ui;
    esp_err_t ui_err = ui.init(hw.panel(), 800, 480);
    if (ui_err != ESP_OK) {
        ESP_LOGE(TAG, "ui.init failed: %d", static_cast<int>(ui_err));
        return;
    }
    ESP_LOGI(TAG, "Dash UI initialized");

    Dash::Controller* controller = nullptr;
    if (can_ready && hw.provider() != nullptr) {
        controller = new Dash::Controller(
            hw.provider(),
            std::unique_ptr<Core::iLockStrategy>(new Core::FreeRTOSLockStrategy()),
            std::unique_ptr<Core::iLockStrategy>(new Core::FreeRTOSLockStrategy()),
            std::unique_ptr<Core::iThreadStrategy>(new Core::FreeRTOSThreadStrategy()));
        controller->start();
    }

    hw.buzz_rtd(1000U, 2000U);

    printf("ESP32 Core Library - Dash UI + Hardware\n");

    while (1) {
        Dash::DataModel snap;
        if (controller != nullptr) {
            snap = controller->snapshot();
        } else {
            // Simple heartbeat animation so display-only mode is obvious.
            static uint32_t t = 0;
            ++t;
            snap.ts_active = true;
            snap.glv_on = true;
            snap.ready_to_drive = (t % 100) < 50;
            snap.pack_voltage_V = 330.0f + static_cast<float>(t % 30);
            snap.pack_current_A = 5.0f;
            snap.motor_rpm = 1000U + (t % 500U);
            snap.speed_kph = 20.0f + static_cast<float>(t % 40U);
            snap.throttle_pct = static_cast<uint8_t>(t % 100U);
        }
        ui.update(snap);
        ui.timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}