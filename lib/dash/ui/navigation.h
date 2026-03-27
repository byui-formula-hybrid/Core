#pragma once

#ifndef ARDUINO_ARCH_NATIVE

#include "lvgl.h"

namespace Dash {
namespace UI {

class Navigation {
public:
    Navigation();

    void init(lv_obj_t* parent);

    lv_obj_t* drive_tab() const { return m_driveTab; }
    lv_obj_t* diagnostics_tab() const { return m_diagnosticsTab; }
    lv_obj_t* faults_tab() const { return m_faultsTab; }
    lv_obj_t* charging_tab() const { return m_chargingTab; }

private:
    lv_obj_t* m_tabview;
    lv_obj_t* m_driveTab;
    lv_obj_t* m_diagnosticsTab;
    lv_obj_t* m_faultsTab;
    lv_obj_t* m_chargingTab;
};

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
