#pragma once

#ifndef ARDUINO_ARCH_NATIVE

#include "lvgl.h"
#include "../data_model.h"

namespace Dash {
namespace UI {

class DrivePage {
public:
    DrivePage();

    void init(lv_obj_t* parent);
    void update(const DataModel& data);

private:
    lv_obj_t* m_root;

    lv_obj_t* m_topLine;
    lv_obj_t* m_statusLine;
    lv_obj_t* m_dataLine;
};

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
