#pragma once

#include <stdint.h>
#include "types.h"

namespace CAN {

class Service {
public: 
    virtual const Result install_driver(const GeneralConfig *g_config, const TimingConfig *t_config, const FilterConfig *f_config) = 0;
    virtual const Result uninstall_driver() = 0;
    virtual const Result start() = 0;
    virtual const Result stop() = 0;
    virtual const Result transmit(const Frame *frame, Tick ticks_to_wait) = 0;
    virtual const Result receive(Frame *frame, Tick ticks_to_wait) = 0;
    virtual const Result alerts(Alert *alerts, Tick ticks_to_wait) = 0;
    virtual const Result reconfigure_alerts(Alert alerts_enabled, Alert *current_alerts) = 0;
    virtual const Result initiate_recovery() = 0;
    virtual const Result status_info(StatusInfo *status_info) = 0;
    virtual const Result clear_transmit_queue() = 0;
    virtual const Result clear_receive_queue() = 0;
    virtual const Result reset_pin(const PIN pin) = 0;

    ~Service() = default;
};

} // namespace CAN