#include <unity.h>
#include <cstdint>
#include "test_main.h"
#include "../../lib/can/provider.h"
#include "../mocks/can/mock_can_service.h"

using namespace CAN;
using namespace MOCKS;

struct Bundle {
    Provider provider;
    MockCanService* service;
};

inline Bundle make_bundle() {
    auto* service = new MockCanService();
    Provider provider(service);

    return { provider, service };
}

void test_can_begin() {
    Bundle bundle = make_bundle();
    bool result = bundle.provider.begin();
    TEST_ASSERT_TRUE(result);
}

void test_can_recover() {
    Bundle bundle = make_bundle();

    bundle.service->on_status_info = [](StatusInfo* status_info) { 
        status_info->state = State::BUS_OFF;
        return Result::OK;
    };

    bool result = bundle.provider.recover();
    TEST_ASSERT_TRUE(result);
}

void test_can_restart() {
    Bundle bundle = make_bundle();

    bundle.service->on_status_info = [](StatusInfo* status_info) { 
        status_info->state = State::STOPPED;
        return Result::OK;
    };

    bool result = bundle.provider.restart();
    TEST_ASSERT_TRUE(result);
}

void test_can_end() {
    Bundle bundle = make_bundle();
    bool result = bundle.provider.end();
    TEST_ASSERT_TRUE(result);
}

void test_can_transmit() {
    Bundle bundle = make_bundle();
    Frame frame;
    bool result = bundle.provider.transmit(frame);
    TEST_ASSERT_TRUE(result);
}

void test_can_receive() {
    Bundle bundle = make_bundle();
    Frame frame;
    bool result = bundle.provider.receive(frame);
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