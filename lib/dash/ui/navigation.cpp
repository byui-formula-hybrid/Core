#ifndef ARDUINO_ARCH_NATIVE

#include "navigation.h"

namespace Dash {
namespace UI {

Navigation::Navigation()
    : m_tabview(nullptr),
      m_driveTab(nullptr),
      m_diagnosticsTab(nullptr),
      m_faultsTab(nullptr),
      m_chargingTab(nullptr) {
}

void Navigation::init(lv_obj_t* parent) {
    lv_obj_t* root = (parent == nullptr) ? lv_scr_act() : parent;

    m_tabview = lv_tabview_create(root, LV_DIR_TOP, 52);
    lv_obj_set_size(m_tabview, LV_PCT(100), LV_PCT(100));

    m_driveTab = lv_tabview_add_tab(m_tabview, "Drive");
    m_diagnosticsTab = lv_tabview_add_tab(m_tabview, "Diagnostics");
    m_faultsTab = lv_tabview_add_tab(m_tabview, "Faults");
    m_chargingTab = lv_tabview_add_tab(m_tabview, "Charging");
}

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
