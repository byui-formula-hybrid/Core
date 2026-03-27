#ifndef ARDUINO_ARCH_NATIVE

#include "ui.h"

#include "lvgl_stub.h"

namespace Dash {
namespace UI {

UI::UI() : m_initialized(false) {
}

esp_err_t UI::init(esp_lcd_panel_handle_t panel, int hor_res, int ver_res) {
    esp_err_t err = dash::ui::init_lvgl_stub(panel, hor_res, ver_res);
    if (err != ESP_OK) {
        return err;
    }

    m_navigation.init(lv_scr_act());
    m_drivePage.init(m_navigation.drive_tab());
    m_diagnosticsPage.init(m_navigation.diagnostics_tab());
    m_faultsPage.init(m_navigation.faults_tab());
    m_chargingPage.init(m_navigation.charging_tab());

    m_initialized = true;
    return ESP_OK;
}

void UI::update(const DataModel& snapshot) {
    if (!m_initialized) {
        return;
    }

    m_drivePage.update(snapshot);
    m_diagnosticsPage.update(snapshot);
    m_faultsPage.update(snapshot);
    m_chargingPage.update(snapshot);
}

void UI::timer_handler() {
    if (!m_initialized) {
        return;
    }

    lv_timer_handler();
}

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
