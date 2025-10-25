#include "core.h"

using namespace CAN;

bool Manager::install_driver() {
    // Reset GPIO pins
    gpio_reset_pin((gpio_num_t)transmit_pin);
    gpio_reset_pin((gpio_num_t)receive_pin);

    // Configure TWAI driver
    twai_timing_config_t t_config = timing_config;
    twai_filter_config_t f_config = filter_config;
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        (gpio_num_t)transmit_pin, 
        (gpio_num_t)receive_pin, 
        TWAI_MODE_NORMAL
    );

    // Install and start TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        // Failed to install TWAI driver
        return false;
    }

    return true;
}

bool Manager::uninstall_driver() {
    return twai_driver_uninstall() != ESP_OK;
}

bool Manager::begin() {
    // If already running, restart first
    if (!end()) {
        // Failed to end previous session
        return false;
    }

    if (!install_driver()) {
        // Failed to install driver
        return false;
    }

    // Start TWAI driver
    if (twai_start() != ESP_OK) {
        // Failed to start TWAI driver
        end();
        return false;
    }

    // Set running state
    is_running = true;
    return true;
}

bool Manager::recover() {
    if (!set_status()) {
        // Failed to get status
        return false;
    }

    switch (status.state) {
        case TWAI_STATE_BUS_OFF:
            return twai_initiate_recovery();
        case TWAI_STATE_STOPPED:
            return true;
        case TWAI_STATE_RECOVERING:
            return true;
        default:
            return false;
    }
}

bool Manager::restart() {
    if (!set_status()) {
        // Failed to get status
        return false;
    }

    switch (status.state) {
        case TWAI_STATE_STOPPED:
            // If stopped, start the driver
            return twai_start();
        default:
            // For other states, restart is not applicable
            return false;
    }
}

bool Manager::end() {
    // Stop and uninstall TWAI driver
    bool did_stop = twai_stop() != ESP_OK;
    bool did_uninstall = uninstall_driver();
    is_running = false;
    return did_stop && did_uninstall;
}

bool Manager::transmit(const Frame& frame, uint32_t timeout) {
    if (twai_transmit(&frame, timeout) != ESP_OK) {
        return false;
    }
    return true;
}

bool Manager::receive(Frame& frame, uint32_t timeout) {
    if (twai_receive(&frame, timeout) != ESP_OK) {
        return false;
    }
    return true;
}