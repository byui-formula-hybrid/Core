#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "display_smoke.h"
#include "lvgl_stub.h"

static const char *TAG = "main";

extern "C" void app_main() {
    ESP_LOGI(TAG, "Display LVGL stub boot");

    esp_err_t init_err = dash::display::init_smoke_display();
    if (init_err != ESP_OK) {
        ESP_LOGE(TAG, "init_smoke_display failed: %d", static_cast<int>(init_err));
        return;
    }

    esp_err_t lvgl_err = dash::ui::init_lvgl_stub(
        dash::display::get_panel_handle(),
        800,
        480);
    if (lvgl_err != ESP_OK) {
        ESP_LOGE(TAG, "init_lvgl_stub failed: %d", static_cast<int>(lvgl_err));
        return;
    }
    ESP_LOGI(TAG, "LVGL stub initialized and rendered");

    printf("ESP32 Core Library - LVGL Stub\n");

    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}