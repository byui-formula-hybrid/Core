/**
 * @file lv_conf.h
 * Configuration file for LVGL v8.3.10
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>
/* screen resolution (added by us) */
#define LV_HOR_RES_MAX   480
#define LV_VER_RES_MAX   480
/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0  /* Swap high/low bytes of RGB565 (tweak if colors look wrong) */

/*====================
   HAL SETTINGS
 *====================*/
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "esp_timer.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR ((uint32_t)(esp_timer_get_time() / 1000))

/*====================
   DEMO USAGE
 *====================*/
#define LV_USE_DEMO_WIDGETS 1

#endif /*LV_CONF_H*/
