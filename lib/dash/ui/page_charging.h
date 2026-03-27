#pragma once

#ifndef ARDUINO_ARCH_NATIVE

#include "lvgl.h"

#include "../data_model.h"

namespace Dash {
namespace UI {

class ChargingPage {
public:
    ChargingPage();

    void init(lv_obj_t* parent);
    void update(const DataModel& data);

private:
    lv_obj_t* m_root;
    lv_obj_t* m_socLine;
    lv_obj_t* m_powerLine;
    lv_obj_t* m_faultLine;
};

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
