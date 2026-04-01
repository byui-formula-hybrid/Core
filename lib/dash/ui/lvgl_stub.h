#ifndef ARDUINO_ARCH_NATIVE

#pragma once

#include "lvgl.h"
#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_types.h"

namespace dash {
namespace ui {

struct LvglPanelContext {
    esp_lcd_panel_handle_t panel = nullptr;
};

// Flush LVGL's rendered area to the RGB panel.
static void panel_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    auto *ctx = static_cast<LvglPanelContext *>(drv->user_data);
    if (ctx == nullptr || ctx->panel == nullptr) {
        lv_disp_flush_ready(drv);
        return;
    }

    esp_lcd_panel_draw_bitmap(
        ctx->panel,
        area->x1,
        area->y1,
        area->x2 + 1,
        area->y2 + 1,
        color_p);

    lv_disp_flush_ready(drv);
}

// Initialize LVGL with a real panel-backed display driver and create a test label.
inline esp_err_t init_lvgl_stub(esp_lcd_panel_handle_t panel, int hor_res, int ver_res)
{
    if (panel == nullptr || hor_res <= 0 || ver_res <= 0) {
        return ESP_ERR_INVALID_ARG;
    }

    lv_init();

    static constexpr int kBufferRows = 40;
    static constexpr int kFallbackBufferRows = 20;
    static lv_disp_draw_buf_t draw_buf;
    static lv_disp_drv_t disp_drv;
    static LvglPanelContext panel_ctx;
    static lv_color_t *buf1 = nullptr;
    static lv_color_t *buf2 = nullptr;

    size_t buf_pixels = static_cast<size_t>(hor_res) * kBufferRows;
    buf1 = static_cast<lv_color_t *>(
        heap_caps_malloc(buf_pixels * sizeof(lv_color_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
    buf2 = static_cast<lv_color_t *>(
        heap_caps_malloc(buf_pixels * sizeof(lv_color_t), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));

    // Fallback for targets without PSRAM.
    if (buf1 == nullptr || buf2 == nullptr) {
        if (buf1 != nullptr) {
            heap_caps_free(buf1);
            buf1 = nullptr;
        }
        if (buf2 != nullptr) {
            heap_caps_free(buf2);
            buf2 = nullptr;
        }

        buf1 = static_cast<lv_color_t *>(
            heap_caps_malloc(buf_pixels * sizeof(lv_color_t), MALLOC_CAP_8BIT));
        buf2 = static_cast<lv_color_t *>(
            heap_caps_malloc(buf_pixels * sizeof(lv_color_t), MALLOC_CAP_8BIT));
    }

    // Last-resort fallback: smaller single buffer.
    if (buf1 == nullptr || buf2 == nullptr) {
        if (buf1 != nullptr) {
            heap_caps_free(buf1);
            buf1 = nullptr;
        }
        if (buf2 != nullptr) {
            heap_caps_free(buf2);
            buf2 = nullptr;
        }

        buf_pixels = static_cast<size_t>(hor_res) * kFallbackBufferRows;
        buf1 = static_cast<lv_color_t *>(
            heap_caps_malloc(buf_pixels * sizeof(lv_color_t), MALLOC_CAP_8BIT));
        buf2 = nullptr;
    }

    if (buf1 == nullptr) {
        return ESP_ERR_NO_MEM;
    }

    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf_pixels);

    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = hor_res;
    disp_drv.ver_res = ver_res;
    disp_drv.flush_cb = panel_flush_cb;
    disp_drv.draw_buf = &draw_buf;
    panel_ctx.panel = panel;
    disp_drv.user_data = &panel_ctx;

    lv_disp_drv_register(&disp_drv);

    // Create a basic label so we can verify LVGL rendering on hardware.
    lv_obj_t *lbl = lv_label_create(lv_scr_act());
    lv_label_set_text(lbl, "DASH INIT OK");
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);

    return ESP_OK;
}

} // namespace ui
} // namespace dash

#endif // ARDUINO_ARCH_NATIVE