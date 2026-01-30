#include <unity.h>

#include <memory>

#include "../../lib/inverter/DTIX50/commands.h"
#include "../../lib/inverter/DTIX50/messages.h"

#include "../../lib/inverter/DTIX50/controller.h"
#include "../mocks/can/mock_can_service.h"
#include "../mocks/strategies/native_lock_strategy.h"
#include "../mocks/strategies/native_thread_strategy.h"

using namespace Inverter;
using namespace MOCKS;

void test_Start_and_Stop() {
    std::shared_ptr<Service> canService(new MockCanService()); // Most likely the ownership should be outside of the class
    std::unique_ptr<Core::iLockStrategy> lockStrategy(new NativeLockStrategy()); // We'll want the class to recieve ownership
    std::unique_ptr<Core::iThreadStrategy> threadStrategy(new NativeThreadStrategy()); // We'll want the class to recieve ownership

    DTIX50::Controller controller(canService, std::move(lockStrategy), std::move(threadStrategy));
    controller.start();

    TEST_ASSERT(controller.started());

    controller.stop();

    TEST_ASSERT(!controller.started());
}

void run_DTIX50_controller_tests() {
    RUN_TEST(test_Start_and_Stop);
}