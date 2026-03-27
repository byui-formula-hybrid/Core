#pragma once

#ifndef ARDUINO_ARCH_NATIVE

#include "lvgl.h"

#include "../data_model.h"

namespace Dash {
namespace UI {

class FaultsPage {
public:
    FaultsPage();

    void init(lv_obj_t* parent);
    void update(const DataModel& data);

private:
    struct FaultEntry {
        uint32_t timestamp_ms;
        const char* source;
        uint8_t code;
    };

    static constexpr int kMaxEntries = 16;

    static void on_item_clicked(lv_event_t* e);
    void push_fault(const char* source, uint8_t code, uint32_t ts_ms);

    lv_obj_t* m_root;
    lv_obj_t* m_list;
    lv_obj_t* m_detail;

    FaultEntry m_entries[kMaxEntries];
    int m_count;
    int m_next;

    bool m_prevAmsFault;
    bool m_prevImdFault;
    bool m_prevPedalFault;
    bool m_prevCanLost;
};

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
