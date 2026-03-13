#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "display_smoke.h"

static const char *TAG = "main";

extern "C" void app_main() {
    ESP_LOGI(TAG, "Display smoke test boot");

    esp_err_t init_err = dash::display::init_smoke_display();
    if (init_err == ESP_OK) {
        ESP_LOGI(TAG, "Panel init OK, drawing bars");
        esp_err_t draw_err = dash::display::draw_color_bars();
        if (draw_err != ESP_OK) {
            ESP_LOGE(TAG, "draw_color_bars failed: %d", static_cast<int>(draw_err));
        }
    } else {
        ESP_LOGE(TAG, "init_smoke_display failed: %d", static_cast<int>(init_err));
    }

    printf("ESP32 Core Library - Display Smoke Test\n");

    const uint16_t colors[] = {
        0xF800, // red
        0x07E0, // green
        0x001F, // blue
        0xFFFF, // white
    };
    size_t idx = 0;
    bool bl_on = true;

    while(1) {
        dash::display::set_backlight(bl_on);
        bl_on = !bl_on;

        dash::display::draw_solid_color(colors[idx]);
        idx = (idx + 1) % (sizeof(colors) / sizeof(colors[0]));

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}