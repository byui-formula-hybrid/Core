#pragma once
#include <functional>
#include <cstdint>
#include "../../../lib/can/service.h"

using namespace CAN;

namespace MOCKS {

class MockCanService : public Service {
public:
    std::function<Result(const GeneralConfig*, const TimingConfig*, const FilterConfig*)>
        on_install_driver =
        [](const GeneralConfig*, const TimingConfig*, const FilterConfig*) { return Result::OK; };

    std::function<Result()> on_uninstall_driver =
        []() { return Result::OK; };

    std::function<Result()> on_start =
        []() { return Result::OK; };

    std::function<Result()> on_stop =
        []() { return Result::OK; };

    std::function<Result(const Frame*, Tick)>
        on_transmit =
        [](const Frame*, Tick) { return Result::OK; };

    std::function<Result(Frame*, Tick)>
        on_receive =
        [](Frame*, Tick) { return Result::OK; };

    std::function<Result(Alert*, Tick)>
        on_alerts =
        [](Alert*, Tick) { return Result::OK; };

    std::function<Result(Alert, Alert*)>
        on_reconfigure_alerts =
        [](Alert, Alert*) { return Result::OK; };

    std::function<Result()> on_initiate_recovery =
        []() { return Result::OK; };

    std::function<Result(StatusInfo*)>
        on_status_info =
        [](StatusInfo*) { return Result::OK; };

    std::function<Result()> on_clear_transmit_queue =
        []() { return Result::OK; };

    std::function<Result()> on_clear_receive_queue =
        []() { return Result::OK; };

    std::function<Result(PIN)>
        on_reset_pin =
        [](PIN) { return Result::OK; };

    struct CallCounts {
        int install_driver = 0;
        int uninstall_driver = 0;
        int start = 0;
        int stop = 0;
        int transmit = 0;
        int receive = 0;
        int alerts = 0;
        int reconfigure_alerts = 0;
        int initiate_recovery = 0;
        int status_info = 0;
        int clear_transmit_queue = 0;
        int clear_receive_queue = 0;
        int reset_pin = 0;
    } calls;

    const Result install_driver(const GeneralConfig *gc,
                                const TimingConfig *tc,
                                const FilterConfig *fc) override
    {
        calls.install_driver++;
        return on_install_driver(gc, tc, fc);
    }

    const Result uninstall_driver() override {
        calls.uninstall_driver++;
        return on_uninstall_driver();
    }

    const Result start() override {
        calls.start++;
        return on_start();
    }

    const Result stop() override {
        calls.stop++;
        return on_stop();
    }

    const Result transmit(const Frame *frame, Tick wait) override {
        calls.transmit++;
        return on_transmit(frame, wait);
    }

    const Result receive(Frame *frame, Tick wait) override {
        calls.receive++;
        return on_receive(frame, wait);
    }

    const Result alerts(Alert *alerts, Tick wait) override {
        calls.alerts++;
        return on_alerts(alerts, wait);
    }

    const Result reconfigure_alerts(Alert a, Alert *c) override {
        calls.reconfigure_alerts++;
        return on_reconfigure_alerts(a, c);
    }

    const Result initiate_recovery() override {
        calls.initiate_recovery++;
        return on_initiate_recovery();
    }

    const Result status_info(StatusInfo *si) override {
        calls.status_info++;
        return on_status_info(si);
    }

    const Result clear_transmit_queue() override {
        calls.clear_transmit_queue++;
        return on_clear_transmit_queue();
    }

    const Result clear_receive_queue() override {
        calls.clear_receive_queue++;
        return on_clear_receive_queue();
    }

    const Result reset_pin(const PIN pin) override {
        calls.reset_pin++;
        return on_reset_pin(pin);
    }
};

}  // namespace MOCKS
