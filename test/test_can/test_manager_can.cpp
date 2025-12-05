#include <unity.h>
#include <cstdint>
#include "test_main.h"
#include "../../lib/can/manager.h"
#include "../mocks/twai/mock_twai_provider.h"

using namespace CAN;
using namespace MOCKS;

struct TestMessage {
    uint64_t a_bool: 1;
    uint64_t padding: 7;
    uint64_t a_uint8: 8;
    uint64_t a_uint16: 16;
    uint64_t a_uint32: 32;
};

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

void test_can_encode() {
    TestMessage message;
    message.a_bool = true;
    message.a_uint8 = 0xAB;
    message.a_uint16 = 0xCDEF;
    message.a_uint32 = 0x12345678;

    Frame frame = encode(1, message);
    
    TEST_ASSERT_EQUAL(1, frame.identifier);
    TEST_ASSERT_EQUAL(8, frame.data_length_code);
    TEST_ASSERT_EQUAL(0x01, frame.data[0]);
    TEST_ASSERT_EQUAL(0xAB, frame.data[1]);
    TEST_ASSERT_EQUAL(0xEF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xCD, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);
}

void test_can_decode() {
    Frame frame;
    frame.identifier = 1;
    frame.data_length_code = 8;
    frame.data[0] = 0b00000001; // a_bool = 1
    frame.data[1] = 0xAB;       // a_uint8 = 0xAB
    frame.data[2] = 0xEF;       // a_uint16 low byte
    frame.data[3] = 0xCD;       // a_uint16 high byte
    frame.data[4] = 0x78;       // a_uint32 byte 0
    frame.data[5] = 0x56;       // a_uint32 byte 1
    frame.data[6] = 0x34;       // a_uint32 byte 2
    frame.data[7] = 0x12;       // a_uint32 byte 3

    Message<TestMessage> message = decode<TestMessage>(&frame);
    
    TEST_ASSERT_EQUAL(1, message.id);
    TEST_ASSERT_EQUAL(true, message.data->a_bool);
    TEST_ASSERT_EQUAL(0xAB, message.data->a_uint8);
    TEST_ASSERT_EQUAL(0xCDEF, message.data->a_uint16);
    TEST_ASSERT_EQUAL(0x12345678, message.data->a_uint32);
}

void run_manager_can_tests() {
    RUN_TEST(test_can_begin);
    RUN_TEST(test_can_recover);
    RUN_TEST(test_can_restart);
    RUN_TEST(test_can_end);
    RUN_TEST(test_can_transmit);
    RUN_TEST(test_can_receive);
    RUN_TEST(test_can_encode);
    RUN_TEST(test_can_decode);
}