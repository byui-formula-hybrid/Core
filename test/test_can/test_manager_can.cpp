#include <unity.h>
#include <cstdint>
#include "test_main.h"
#include "../../lib/can/manager.h"
#include "../mocks/twai/mock_twai_provider.h"

using namespace CAN;
using namespace MOCKS;

struct ManagerBundle {
    Manager manager;
    MockTwaiProvider* provider;
};

inline ManagerBundle make_manager() {
    auto* mock = new MockTwaiProvider();
    Manager manager(mock);

    return { manager, mock };
}

void test_can_begin() {
    ManagerBundle bundle = make_manager();
    bool result = bundle.manager.begin();
    TEST_ASSERT_TRUE(result);
}

void test_can_recover() {
    ManagerBundle bundle = make_manager();

    bundle.provider->on_status_info = [](StatusInfo* status_info) { 
        status_info->state = State::BUS_OFF;
        return Result::OK;
    };

    bool result = bundle.manager.recover();
    TEST_ASSERT_TRUE(result);
}

void test_can_restart() {
    ManagerBundle bundle = make_manager();

    bundle.provider->on_status_info = [](StatusInfo* status_info) { 
        status_info->state = State::STOPPED;
        return Result::OK;
    };

    bool result = bundle.manager.restart();
    TEST_ASSERT_TRUE(result);
}

void test_can_end() {
    ManagerBundle bundle = make_manager();
    bool result = bundle.manager.end();
    TEST_ASSERT_TRUE(result);
}

void test_can_transmit() {
    ManagerBundle bundle = make_manager();
    Frame frame;
    bool result = bundle.manager.transmit(frame);
    TEST_ASSERT_TRUE(result);
}

void test_can_receive() {
    ManagerBundle bundle = make_manager();
    Frame frame;
    bool result = bundle.manager.receive(frame);
    TEST_ASSERT_TRUE(result);
}

void run_manager_can_tests() {
    RUN_TEST(test_can_begin);
    RUN_TEST(test_can_recover);
    RUN_TEST(test_can_restart);
    RUN_TEST(test_can_end);
    RUN_TEST(test_can_transmit);
    RUN_TEST(test_can_receive);
}