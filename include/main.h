#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

// STD APIs
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// Platform-specific APIs
#ifdef ESP_PLATFORM
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#endif

#ifdef ENV_STM32
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#if defined(ENV_STM32F7)
#include "stm32f7defs.h"
#endif
#endif

int _write(int file, char *ptr, int len);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
