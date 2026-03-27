#pragma once

#include "../data_model.h"
#include "../pedal_errors.h"

#ifndef ARDUINO_ARCH_NATIVE
#include "lvgl.h"
#endif

namespace Dash {
namespace UI {

inline const char* pedal_error_to_text(const Dash::DataModel& data) {
    if (data.pedal_error_code > 0) {
        return Dash::PedalErrors::decode(data.pedal_error_code);
    }

    if (data.pedal_sync_fault) {
        return "Pedal Implausible";
    }

    return "";
}

#ifndef ARDUINO_ARCH_NATIVE

class DiagnosticsPage {
public:
    DiagnosticsPage();

    void init(lv_obj_t* parent);
    void update(const DataModel& data);

private:
    lv_obj_t* m_root;
    lv_obj_t* m_summaryLine;
    lv_obj_t* m_pedalLine;
    lv_obj_t* m_canLine;
};

#endif

}  // namespace UI
}  // namespace Dash
