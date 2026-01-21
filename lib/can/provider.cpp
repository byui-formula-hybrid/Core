#include "provider.h"

using namespace CAN;

bool Provider::set_status() {
    return service->status_info(&status) == Result::OK;
}

bool Provider::install_driver() {
    // Reset GPIO pins
    service->reset_pin(transmit_pin);
    service->reset_pin(receive_pin);

    // Configure TWAI driver
    TimingConfig t_config = timing_config;
    FilterConfig f_config = filter_config;
    GeneralConfig g_config = GeneralConfig(transmit_pin, receive_pin, Mode::NORMAL);

    // Install and start TWAI driver
    if (service->install_driver(&g_config, &t_config, &f_config) != Result::OK) {
        // Failed to install TWAI driver
        return false;
    }

    return true;
}

bool Provider::uninstall_driver() {
    return service->uninstall_driver() == Result::OK;
}

bool Provider::begin() {
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
    if (service->start() != Result::OK) {
        // Failed to start TWAI driver
        end();
        return false;
    }

    // Set running state
    is_running = true;
    return true;
}

bool Provider::recover() {
    if (!set_status()) {
        // Failed to get status
        return false;
    }

    switch (status.state) {
        case State::BUS_OFF:
            return service->initiate_recovery() == Result::OK;
        case State::STOPPED:
            return true;
        case State::RECOVERING:
            return true;
        default:
            return false;
    }
}

bool Provider::restart() {
    if (!set_status()) {
        // Failed to get status
        return false;
    }

    switch (status.state) {
        case State::STOPPED:
            // If stopped, start the driver
            return service->start() == Result::OK;
        default:
            // For other states, restart is not applicable
            return false;
    }
}

bool Provider::end() {
    // Stop and uninstall TWAI driver
    bool did_stop = service->stop() == Result::OK;
    bool did_uninstall = uninstall_driver();
    is_running = false;
    return did_stop && did_uninstall;
}

bool Provider::transmit(const Frame& frame, uint32_t timeout) {
    if (service->transmit(&frame, timeout) != Result::OK) {
        return false;
    }
    return true;
}

bool Provider::receive(Frame& frame, uint32_t timeout) {
    if (service->receive(&frame, timeout) != Result::OK) {
        return false;
    }
    return true;
}