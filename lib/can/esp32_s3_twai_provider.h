#pragma once

#include "twai_provider.h"
#include <driver/twai.h>
#include <driver/gpio.h>

using namespace TWAI;

namespace CAN {

class ESP32S3TwaiProvider : public TwaiProvider {
public:

    const Result install_driver(
        const GeneralConfig *g_config,
        const TimingConfig *t_config,
        const FilterConfig *f_config
    ) override {
        return (Result)twai_driver_install(
            (twai_general_config_t*)g_config,
            (twai_timing_config_t*)t_config,
            (twai_filter_config_t*)f_config
        );
    }

    const Result uninstall_driver() override {
        return (Result)twai_driver_uninstall();
    }

    const Result start() override {
        return (Result)twai_start();
    }

    const Result stop() override {
        return (Result)twai_stop();
    }

    const Result transmit(const Frame *frame, Tick ticks_to_wait) override {
        return (Result)twai_transmit((twai_message_t*)frame, ticks_to_wait);
    }

    const Result receive(Frame *frame, Tick ticks_to_wait) override {
        return (Result)twai_receive((twai_message_t*)frame, ticks_to_wait);
    }

    const Result alerts(Alert *alerts, Tick ticks_to_wait) override {
        return (Result)twai_read_alerts(alerts, ticks_to_wait);
    }

    const Result reconfigure_alerts(Alert alerts_to_enable, Alert *current_alerts) override {
        return (Result)twai_reconfigure_alerts(alerts_to_enable, current_alerts);
    }

    const Result initiate_recovery() override {
        return (Result)twai_initiate_recovery();
    }

    const Result status_info(StatusInfo *status_info) override {
        return (Result)twai_get_status_info((twai_status_info_t*)status_info);
    }

    const Result clear_transmit_queue() override {
        return (Result)twai_clear_transmit_queue();
    }

    const Result clear_receive_queue() override {
        return (Result)twai_clear_receive_queue();
    }

    const Result reset_pin(const PIN pin) override {
        return (Result)gpio_reset_pin((gpio_num_t)pin);
    }
};

} // namespace CAN
