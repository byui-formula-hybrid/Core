#include "manager.h"

using namespace CAN;
using namespace TWAI;

bool Manager::set_status() {
    return provider->status_info(&status) == Result::OK;
}

bool Manager::install_driver() {
    // Reset GPIO pins
    provider->reset_pin(transmit_pin);
    provider->reset_pin(receive_pin);

    // Configure TWAI driver
    TimingConfig t_config = timing_config;
    FilterConfig f_config = filter_config;
    GeneralConfig g_config = GeneralConfig(transmit_pin, receive_pin, Mode::NORMAL);

    // Install and start TWAI driver
    if (provider->install_driver(&g_config, &t_config, &f_config) != Result::OK) {
        // Failed to install TWAI driver
        return false;
    }

    return true;
}

bool Manager::uninstall_driver() {
    return provider->uninstall_driver() == Result::OK;
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
    if (provider->start() != Result::OK) {
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
        case State::BUS_OFF:
            return provider->initiate_recovery() == Result::OK;
        case State::STOPPED:
            return true;
        case State::RECOVERING:
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
        case State::STOPPED:
            // If stopped, start the driver
            return provider->start() == Result::OK;
        default:
            // For other states, restart is not applicable
            return false;
    }
}

bool Manager::end() {
    // Stop and uninstall TWAI driver
    bool did_stop = provider->stop() == Result::OK;
    bool did_uninstall = uninstall_driver();
    is_running = false;
    return did_stop && did_uninstall;
}

bool Manager::transmit(const Frame& frame, uint32_t timeout) {
    if (provider->transmit(&frame, timeout) != Result::OK) {
        return false;
    }
    return true;
}

bool Manager::receive(Frame& frame, uint32_t timeout) {
    if (provider->receive(&frame, timeout) != Result::OK) {
        return false;
    }
    return true;
}