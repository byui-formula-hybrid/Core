#ifndef ARDUINO_ARCH_NATIVE

#include "page_charging.h"

#include <cstdio>

namespace Dash {
namespace UI {

ChargingPage::ChargingPage()
    : m_root(nullptr),
      m_socLine(nullptr),
      m_powerLine(nullptr),
      m_faultLine(nullptr) {
}

void ChargingPage::init(lv_obj_t* parent) {
    m_root = parent;

    m_socLine = lv_label_create(m_root);
    lv_obj_set_width(m_socLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_socLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_socLine, LV_ALIGN_TOP_LEFT, 12, 12);

    m_powerLine = lv_label_create(m_root);
    lv_obj_set_width(m_powerLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_powerLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_powerLine, LV_ALIGN_TOP_LEFT, 12, 54);

    m_faultLine = lv_label_create(m_root);
    lv_obj_set_width(m_faultLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_faultLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_faultLine, LV_ALIGN_TOP_LEFT, 12, 96);
}

void ChargingPage::update(const DataModel& data) {
    if (m_root == nullptr) {
        return;
    }

    char line[220];

    std::snprintf(line, sizeof(line), "SOC:N/A | TS:%s | GLV:%s",
        data.ts_active ? "ON" : "OFF",
        data.glv_on ? "ON" : "OFF");
    lv_label_set_text(m_socLine, line);

    std::snprintf(line, sizeof(line), "Charge V/I: %.1fV / %.1fA",
        static_cast<double>(data.pack_voltage_V),
        static_cast<double>(data.pack_current_A));
    lv_label_set_text(m_powerLine, line);

    std::snprintf(line, sizeof(line), "AMS:%s | IMD:%s | CAN:%s",
        data.has_ams_fault() ? "FAULT" : "OK",
        data.imd_fault ? "FAULT" : "OK",
        data.can_comm_lost ? "COMM LOST" : "OK");
    lv_label_set_text(m_faultLine, line);
}

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
