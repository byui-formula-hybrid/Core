#ifndef ARDUINO_ARCH_NATIVE

#include "page_faults.h"

#include <cstdio>

namespace Dash {
namespace UI {

FaultsPage::FaultsPage()
    : m_root(nullptr),
      m_list(nullptr),
      m_detail(nullptr),
      m_count(0),
      m_next(0),
      m_prevAmsFault(false),
      m_prevImdFault(false),
      m_prevPedalFault(false),
      m_prevCanLost(false) {
}

void FaultsPage::init(lv_obj_t* parent) {
    m_root = parent;

    m_list = lv_list_create(m_root);
    lv_obj_set_size(m_list, LV_PCT(100), 210);
    lv_obj_align(m_list, LV_ALIGN_TOP_LEFT, 0, 0);

    m_detail = lv_label_create(m_root);
    lv_label_set_text(m_detail, "Tap a fault entry for details");
    lv_obj_set_width(m_detail, LV_PCT(100));
    lv_obj_set_style_text_align(m_detail, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_align(m_detail, LV_ALIGN_BOTTOM_LEFT, 12, -12);
}

void FaultsPage::on_item_clicked(lv_event_t* e) {
    lv_obj_t* item = lv_event_get_target(e);
    if (item == nullptr) {
        return;
    }

    lv_obj_t* list = lv_obj_get_parent(item);
    if (list == nullptr) {
        return;
    }

    FaultsPage* self = static_cast<FaultsPage*>(lv_event_get_user_data(e));
    if (self == nullptr || self->m_detail == nullptr) {
        return;
    }

    const char* item_text = lv_list_get_btn_text(list, item);
    if (item_text == nullptr) {
        return;
    }

    char line[220];
    std::snprintf(line, sizeof(line), "Selected: %s", item_text);
    lv_label_set_text(self->m_detail, line);
}

void FaultsPage::push_fault(const char* source, uint8_t code, uint32_t ts_ms) {
    if (m_list == nullptr) {
        return;
    }

    m_entries[m_next] = FaultEntry{ts_ms, source, code};
    m_next = (m_next + 1) % kMaxEntries;
    if (m_count < kMaxEntries) {
        ++m_count;
    }

    char text[96];
    std::snprintf(text, sizeof(text), "t=%lu %s (code=%u)",
        static_cast<unsigned long>(ts_ms),
        source,
        static_cast<unsigned int>(code));

    lv_obj_t* btn = lv_list_add_btn(m_list, nullptr, text);
    lv_obj_add_event_cb(btn, on_item_clicked, LV_EVENT_CLICKED, this);
}

void FaultsPage::update(const DataModel& data) {
    if (m_root == nullptr) {
        return;
    }

    const bool has_ams = data.has_ams_fault();
    const bool has_pedal_fault = (data.pedal_error_code > 0) || data.pedal_sync_fault;

    if (has_ams && !m_prevAmsFault) {
        push_fault("AMS", 1, data.last_heartbeat_ms);
    }

    if (data.imd_fault && !m_prevImdFault) {
        push_fault("IMD", 1, data.last_heartbeat_ms);
    }

    if (has_pedal_fault && !m_prevPedalFault) {
        push_fault("Pedal", data.pedal_error_code, data.last_heartbeat_ms);
    }

    if (data.can_comm_lost && !m_prevCanLost) {
        push_fault("CAN", 1, data.last_heartbeat_ms);
    }

    m_prevAmsFault = has_ams;
    m_prevImdFault = data.imd_fault;
    m_prevPedalFault = has_pedal_fault;
    m_prevCanLost = data.can_comm_lost;
}

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
