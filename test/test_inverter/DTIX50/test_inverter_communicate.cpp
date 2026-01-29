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
    MockCanService* canService;
    canService = new MockCanService();
    DTIX50::Controller controller(canService, std::unique_ptr<Core::iLockStrategy>(new NativeLockStrategy()), std::unique_ptr<Core::iThreadStrategy>(new nativeThreadStrategy()));
    controller.start();

    TEST_ASSERT(controller.started());

    controller.stop();

    TEST_ASSERT(!controller.started());
}

void run_DTIX50_controller_tests() {
    RUN_TEST(test_Start_and_Stop);
}