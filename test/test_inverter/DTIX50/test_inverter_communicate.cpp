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
    std::shared_ptr<Service> canService(new MockCanService()); // Most likely the ownership should be outside of the class
    std::unique_ptr<Core::iLockStrategy> lockStrategy(new NativeLockStrategy()); // We'll want the class to recieve ownership
    std::unique_ptr<Core::iThreadStrategy> threadStrategy(new NativeThreadStrategy()); // We'll want the class to recieve ownership

    DTIX50::Heartbeat heartbeat(canService, std::move(lockStrategy), std::move(threadStrategy));
    heartbeat.start();

    TEST_ASSERT(heartbeat.started());

    // We should always get at least 3 transmits
    std::this_thread::sleep_for(std::chrono::seconds(1));
    TEST_ASSERT_GREATER_OR_EQUAL(3, std::dynamic_pointer_cast<MockCanService>(canService)->calls.transmit);

    heartbeat.stop();

    TEST_ASSERT(!heartbeat.started());
}

void run_DTIX50_controller_tests() {
    RUN_TEST(test_Heartbeat);
}