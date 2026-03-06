#include <unity.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "../../lib/can/provider.h"
#include "../../lib/can/service.h"
#include "../../lib/dash/controller.h"
#include "../../lib/dash/messages.h"
#include "../mocks/can/mock_can_service.h"
#include "../mocks/strategies/native_lock_strategy.h"
#include "../mocks/strategies/native_thread_strategy.h"

using namespace Dash;
using namespace CAN;
using namespace MOCKS;

static std::shared_ptr<Provider> create_provider(MockCanService* service) {
    return std::shared_ptr<Provider>(new Provider((Service*)service));
}

void test_dash_controller_snapshot_defaults() {
    MockCanService* canService = new MockCanService();
    auto provider = create_provider(canService);

    Controller controller(
        provider,
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iThreadStrategy>(new NativeThreadStrategy())
    );

    DataModel data = controller.snapshot();

    TEST_ASSERT_EQUAL_FLOAT(0.0f, data.pack_voltage_V);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, data.pack_current_A);
    TEST_ASSERT_EQUAL(0, data.motor_rpm);
    TEST_ASSERT_FALSE(data.has_ams_fault());
    TEST_ASSERT_FALSE(data.imd_fault);
    TEST_ASSERT_FALSE(data.can_comm_lost);

    delete canService;
}

void test_dash_controller_decodes_voltage_current() {
    MockCanService* canService = new MockCanService();
    auto provider = create_provider(canService);

    AMSVoltCurrent vc = {4200, 500, 0};
    Frame vc_frame(CANID::AMSVoltCurrent, &vc);
    std::atomic<int> call_count{0};

    canService->on_receive = [&call_count, &vc_frame](Frame* frame, Tick) {
        int count = call_count.fetch_add(1);
        if (count == 0) {
            *frame = vc_frame;
            return Result::OK;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        return Result::ERR_TIMEOUT;
    };

    Controller controller(
        provider,
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iThreadStrategy>(new NativeThreadStrategy())
    );

    controller.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    DataModel data = controller.snapshot();
    controller.stop();

    TEST_ASSERT_EQUAL_FLOAT(420.0f, data.pack_voltage_V);
    TEST_ASSERT_EQUAL_FLOAT(50.0f, data.pack_current_A);

    delete canService;
}

void test_dash_controller_latches_ams_fault() {
    MockCanService* canService = new MockCanService();
    auto provider = create_provider(canService);

    AMSFault fault = {1, 0, 0, 0, 0, 0, 0, 0};
    AMSFault clear = {0, 0, 0, 0, 0, 0, 0, 0};
    Frame f1(CANID::AMSFault, &fault);
    Frame f2(CANID::AMSFault, &clear);
    std::atomic<int> call_count{0};

    canService->on_receive = [&call_count, &f1, &f2](Frame* frame, Tick) {
        int count = call_count.fetch_add(1);
        if (count == 0) {
            *frame = f1;
            return Result::OK;
        }
        if (count == 1) {
            *frame = f2;
            return Result::OK;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        return Result::ERR_TIMEOUT;
    };

    Controller controller(
        provider,
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iThreadStrategy>(new NativeThreadStrategy())
    );

    controller.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    DataModel data = controller.snapshot();
    controller.stop();

    TEST_ASSERT_TRUE(data.ams_over_voltage);
    TEST_ASSERT_TRUE(data.has_ams_fault());

    delete canService;
}

void test_dash_controller_can_timeout_and_recovery() {
    MockCanService* canService = new MockCanService();
    auto provider = create_provider(canService);

    Heartbeat hb = {1, 0};
    Frame heartbeat_frame(CANID::Heartbeat, &hb);
    std::atomic<int> state{0};

    canService->on_receive = [&state, &heartbeat_frame](Frame* frame, Tick) {
        int s = state.load();
        if (s == 0) {
            state.store(1);
            *frame = heartbeat_frame;
            return Result::OK;
        }
        if (s == 2) {
            state.store(3);
            *frame = heartbeat_frame;
            return Result::OK;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return Result::ERR_TIMEOUT;
    };

    Controller controller(
        provider,
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iThreadStrategy>(new NativeThreadStrategy())
    );

    controller.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(700));
    DataModel timed_out = controller.snapshot();

    state.store(2);
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    DataModel recovered = controller.snapshot();

    controller.stop();

    TEST_ASSERT_TRUE(timed_out.can_comm_lost);
    TEST_ASSERT_FALSE(recovered.can_comm_lost);

    delete canService;
}

void test_dash_controller_pedal_error_persistence() {
    MockCanService* canService = new MockCanService();
    auto provider = create_provider(canService);

    PedalData with_error = {50, 0, 0, 2000, 2000, 5, 0, 0};
    PedalData clear_error = {50, 0, 0, 2000, 2000, 0, 0, 0};
    Frame error_frame(CANID::PedalData, &with_error);
    Frame clear_frame(CANID::PedalData, &clear_error);
    std::atomic<int> call_count{0};

    canService->on_receive = [&call_count, &error_frame, &clear_frame](Frame* frame, Tick) {
        int count = call_count.fetch_add(1);
        if (count == 0) {
            *frame = error_frame;
            return Result::OK;
        }

        *frame = clear_frame;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return Result::OK;
    };

    Controller controller(
        provider,
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()),
        std::unique_ptr<Core::iThreadStrategy>(new NativeThreadStrategy())
    );

    controller.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    DataModel persisted = controller.snapshot();

    std::this_thread::sleep_for(std::chrono::milliseconds(1800));
    DataModel cleared = controller.snapshot();

    controller.stop();

    TEST_ASSERT_EQUAL(5, persisted.pedal_error_code);
    TEST_ASSERT_EQUAL(0, cleared.pedal_error_code);

    delete canService;
}

void run_dash_controller_tests() {
    RUN_TEST(test_dash_controller_snapshot_defaults);
    RUN_TEST(test_dash_controller_decodes_voltage_current);
    RUN_TEST(test_dash_controller_latches_ams_fault);
    RUN_TEST(test_dash_controller_can_timeout_and_recovery);
    RUN_TEST(test_dash_controller_pedal_error_persistence);
}
