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

    if (hw.init_can() != ESP_OK) {
        ESP_LOGE(TAG, "can init failed");
        return;
    }

    if (hw.init_touch() != ESP_OK) {
        ESP_LOGE(TAG, "touch init failed");
        return;
    }

    Dash::UI::UI ui;
    esp_err_t ui_err = ui.init(hw.panel(), 800, 480);
    if (ui_err != ESP_OK) {
        ESP_LOGE(TAG, "ui.init failed: %d", static_cast<int>(ui_err));
        return;
    }
    ESP_LOGI(TAG, "Dash UI initialized");

    Dash::Controller controller(
        hw.provider(),
        std::unique_ptr<Core::iLockStrategy>(new Core::FreeRTOSLockStrategy()),
        std::unique_ptr<Core::iLockStrategy>(new Core::FreeRTOSLockStrategy()),
        std::unique_ptr<Core::iThreadStrategy>(new Core::FreeRTOSThreadStrategy()));
    controller.start();

    hw.buzz_rtd(1000U, 2000U);

    printf("ESP32 Core Library - Dash UI + Hardware\n");

    while (1) {
        Dash::DataModel snap = controller.snapshot();
        ui.update(snap);
        ui.timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}