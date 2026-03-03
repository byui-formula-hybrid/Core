#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

// STD APIs
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

// ESP32 APIs and Environments
#if defined(ENV_ESP32)
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// STM32 APIs and Environments
#elif defined(ENV_STM32)
#include "FreeRTOS.h"
#include "task.h"
#if defined(ENV_STM32F7)
#include "stm32f7defs.h"


// void CycleLEDs(void);
#endif

int _write(int file, char *ptr, int len);
void Error_Handler(void);
#endif


#ifdef __cplusplus
}
#endif


#endif /* __MAIN_H */
