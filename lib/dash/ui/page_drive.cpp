#ifndef ARDUINO_ARCH_NATIVE

#include "page_drive.h"

#include <cstdio>

namespace Dash {
namespace UI {

namespace {

const char* on_off(bool value) {
    return value ? "ON" : "OFF";
}

const char* yes_no(bool value) {
    return value ? "YES" : "NO";
}

const char* pedal_fault_text(const DataModel& data) {
    return (data.pedal_error_code > 0 || data.pedal_sync_fault) ? "FAULT" : "OK";
}

}  // namespace

DrivePage::DrivePage()
    : m_root(nullptr),
      m_topLine(nullptr),
      m_statusLine(nullptr),
      m_dataLine(nullptr) {
}

void DrivePage::init(lv_obj_t* parent) {
    m_root = parent;

    m_topLine = lv_label_create(m_root);
    lv_obj_set_width(m_topLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_topLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_topLine, LV_ALIGN_TOP_LEFT, 12, 12);

    m_statusLine = lv_label_create(m_root);
    lv_obj_set_width(m_statusLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_statusLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_statusLine, LV_ALIGN_TOP_LEFT, 12, 54);

    m_dataLine = lv_label_create(m_root);
    lv_obj_set_width(m_dataLine, LV_PCT(100));
    lv_obj_set_style_text_align(m_dataLine, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_dataLine, LV_ALIGN_TOP_LEFT, 12, 96);
}

void DrivePage::update(const DataModel& data) {
    if (m_root == nullptr) {
        return;
    }

    char line[220];

    std::snprintf(
        line,
        sizeof(line),
        "TS:%s | GLV:%s | RTD:%s",
        on_off(data.ts_active),
        on_off(data.glv_on),
        yes_no(data.ready_to_drive));
    lv_label_set_text(m_topLine, line);

    std::snprintf(
        line,
        sizeof(line),
        "AMS:%s | IMD:%s | Pedal:%s | CAN:%s",
        data.has_ams_fault() ? "FAULT" : "OK",
        data.imd_fault ? "FAULT" : "OK",
        pedal_fault_text(data),
        data.can_comm_lost ? "COMM LOST" : "OK");
    lv_label_set_text(m_statusLine, line);

    std::snprintf(
        line,
        sizeof(line),
        "V:%.1fV | I:%.1fA | Spd:%.1fkph | RPM:%lu | Thr:%u%% | Brk:%s",
        static_cast<double>(data.pack_voltage_V),
        static_cast<double>(data.pack_current_A),
        static_cast<double>(data.speed_kph),
        static_cast<unsigned long>(data.motor_rpm),
        static_cast<unsigned int>(data.throttle_pct),
        on_off(data.brake_pressed));
    lv_label_set_text(m_dataLine, line);
}

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
