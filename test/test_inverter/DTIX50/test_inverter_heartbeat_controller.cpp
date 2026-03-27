#include <unity.h>

#include <memory>
#include <thread>
#include <chrono>

#include "../../lib/inverter/DTIX50/commands.h"
#include "../../lib/inverter/DTIX50/messages.h"

#include "../../lib/inverter/DTIX50/heartbeat.h"
#include "../mocks/can/mock_can_service.h"
#include "../mocks/strategies/native_lock_strategy.h"
#include "../mocks/strategies/native_thread_strategy.h"

using namespace Inverter;
using namespace MOCKS;

void test_Heartbeat() {
    MockCanService* canService = new MockCanService(); // Most likely the ownership should be outside of the class
    int drive_type[2] = {0, 0};
    canService->on_transmit = [&drive_type](const Frame* frame, Tick tick){ 
        if(frame->data[0] == 1)
            drive_type[0]++; 
        else if (frame->data[0] == 0)
            drive_type[1]++;
        return Result::OK; 
    };
    std::shared_ptr<Provider> canProvider(new Provider((Service*)canService, PIN::NUM_12, PIN::NUM_14));
    std::unique_ptr<Core::iLockStrategy> lockStrategy(new NativeLockStrategy()); // We'll want the class to recieve ownership
    std::unique_ptr<Core::iThreadStrategy> threadStrategy(new NativeThreadStrategy()); // We'll want the class to recieve ownership

    DTIX50::Heartbeat heartbeat(canProvider, std::move(lockStrategy), std::move(threadStrategy), nullptr);
    
    TEST_ASSERT(!heartbeat.started());
    heartbeat.start();
    TEST_ASSERT(heartbeat.started());

    // We should always get at least 3 transmits
    std::this_thread::sleep_for(std::chrono::seconds(1));
    TEST_ASSERT_GREATER_OR_EQUAL(3, ((MockCanService*)canService)->calls.transmit);

    TEST_ASSERT_GREATER_OR_EQUAL(2, drive_type[0]); // Verify that we have sent at least 2 drive enables.

    heartbeat.stop();

    TEST_ASSERT(!heartbeat.started());

    TEST_ASSERT_EQUAL(1, drive_type[1]); // Verify that only one drive disable has been sent

    free(canService);
}

void run_DTIX50_controller_tests() {
    RUN_TEST(test_Heartbeat);
}