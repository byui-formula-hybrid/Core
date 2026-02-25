#ifndef ARDUINO_ARCH_NATIVE

#pragma once

#include "lvgl.h"

namespace dash {
namespace ui {

// dummy flush callback - immediately report ready so LVGL can continue
static void stub_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
    (void)drv;
    (void)area;
    (void)color_p;

    // nothing to draw, just tell LVGL we're done
    lv_disp_flush_ready(drv);
}

// initialize LVGL with a fake display driver and create a single label
inline void init_lvgl_stub()
{
    lv_init();

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = stub_flush_cb;

    lv_disp_drv_register(&disp_drv);

    // create a basic label so we can verify LVGL objects work
    lv_obj_t *lbl = lv_label_create(lv_scr_act());
    lv_label_set_text(lbl, "DASH INIT OK");
}

} // namespace ui
} // namespace dash

#endif // ARDUINO_ARCH_NATIVE