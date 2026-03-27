#pragma once

#ifndef ARDUINO_ARCH_NATIVE

#include "esp_err.h"
#include "esp_lcd_types.h"

#include "../data_model.h"
#include "navigation.h"
#include "page_charging.h"
#include "page_diagnostics.h"
#include "page_drive.h"
#include "page_faults.h"

namespace Dash {
namespace UI {

class UI {
public:
    UI();

    esp_err_t init(esp_lcd_panel_handle_t panel, int hor_res, int ver_res);
    void update(const DataModel& snapshot);
    void timer_handler();

private:
    bool m_initialized;
    Navigation m_navigation;
    DrivePage m_drivePage;
    DiagnosticsPage m_diagnosticsPage;
    FaultsPage m_faultsPage;
    ChargingPage m_chargingPage;
};

}  // namespace UI
}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
