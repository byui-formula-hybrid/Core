#ifndef ARDUINO_ARCH_NATIVE

#include "page_diagnostics.h"

#include <cstdio>

namespace Dash {
namespace UI {

DiagnosticsPage::DiagnosticsPage()
    : m_root(nullptr),
      m_summaryLine(nullptr),
      m_pedalLine(nullptr),
      m_canLine(nullptr) {
}

void DiagnosticsPage::init(lv_obj_t* parent) {
    m_root = parent;

    m_summaryLine = lv_label_create(m_root);
    lv_obj_set_width(m_summaryLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_summaryLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_summaryLine, LV_ALIGN_TOP_LEFT, 12, 12);

    m_pedalLine = lv_label_create(m_root);
    lv_obj_set_width(m_pedalLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_pedalLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_pedalLine, LV_ALIGN_TOP_LEFT, 12, 54);

    m_canLine = lv_label_create(m_root);
    lv_obj_set_width(m_canLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_canLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_canLine, LV_ALIGN_TOP_LEFT, 12, 96);
}

void DiagnosticsPage::update(const DataModel& data) {
    if (m_root == nullptr) {
        return;
    }

    char line[220];

    std::snprintf(
        line,
        sizeof(line),
        "CellV min/max:N/A | MaxTemp:%.1fC | Pack:%.1fV %.1fA",
        static_cast<double>(data.max_cell_temp_C),
        static_cast<double>(data.pack_voltage_V),
        static_cast<double>(data.pack_current_A));
    lv_label_set_text(m_summaryLine, line);

    std::snprintf(
        line,
        sizeof(line),
        "Pedal ADC1:%u ADC2:%u | Fault:%s",
        static_cast<unsigned int>(data.pedal_adc1),
        static_cast<unsigned int>(data.pedal_adc2),
        pedal_error_to_text(data));
    lv_label_set_text(m_pedalLine, line);

    std::snprintf(
        line,
        sizeof(line),
        "CAN HB:%lu ms | Status:%s",
        static_cast<unsigned long>(data.last_heartbeat_ms),
        data.can_comm_lost ? "COMM LOST" : "OK");
    lv_label_set_text(m_canLine, line);
}

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
