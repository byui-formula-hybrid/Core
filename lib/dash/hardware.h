#pragma once

#ifndef ARDUINO_ARCH_NATIVE

#include <memory>

#include "esp_err.h"
#include "esp_lcd_types.h"

#include "../can/provider.h"

namespace Dash {

class Hardware {
public:
    Hardware();

    esp_err_t init_can();
    esp_err_t init_display();
    esp_err_t init_touch();

    esp_err_t buzz_rtd(uint32_t duration_ms = 1500U, uint32_t freq_hz = 2000U);

    std::shared_ptr<CAN::Provider> provider() const { return m_provider; }
    esp_lcd_panel_handle_t panel() const { return m_panel; }

private:
    std::shared_ptr<CAN::Provider> m_provider;
    esp_lcd_panel_handle_t m_panel;
};

}  // namespace Dash

#endif  // ARDUINO_ARCH_NATIVE
