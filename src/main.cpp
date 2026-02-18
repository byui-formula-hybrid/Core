#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "lv_conf.h"

extern "C" void app_main() {
    // Initialize LVGL
    lv_init();

    // Simple log to prove it works
    printf("ESP32 Core Library - Race Car\n");

    while(1) {
        // Keep the task alive
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}