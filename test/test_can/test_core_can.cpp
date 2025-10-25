#include <unity.h>
#include "test_main.h"
#include "../../lib/can/core.h"
#include "../mocks/test_can_message.h"

using namespace CAN;

// TODO: Mock TWAI functions for isolated testing

void test_can_set_status() {
    Manager can_manager;
    bool result = can_manager.set_status();
    TEST_ASSERT_TRUE(result);
}

void test_can_begin() {
    Manager can_manager;
    bool result = can_manager.begin();
    TEST_ASSERT_TRUE(result);
}

void test_can_recover() {
    Manager can_manager;
    bool result = can_manager.recover();
    TEST_ASSERT_TRUE(result);
}

void test_can_restart() {
    Manager can_manager;
    bool result = can_manager.restart();
    TEST_ASSERT_TRUE(result);
}

void test_can_end() {
    Manager can_manager;
    bool result = can_manager.end();
    TEST_ASSERT_TRUE(result);
}

void test_can_transmit() {
    Manager can_manager;
    Frame frame;
    bool result = can_manager.transmit(frame);
    TEST_ASSERT_TRUE(result);
}

void test_can_receive() {
    Manager can_manager;
    Frame frame;
    bool result = can_manager.receive(frame);
    TEST_ASSERT_TRUE(result);
}

void test_can_encode() {
    Manager can_manager;
    TestCANMessage message;
    message.a_bool = true;
    message.a_uint8 = 0xAB;
    message.a_uint16 = 0xCDEF;
    message.a_uint32 = 0x12345678;

    Frame frame = encode(1, message);
    
    TEST_ASSERT_EQUAL(1, frame.identifier);
    TEST_ASSERT_EQUAL(8, frame.data_length_code);
    TEST_ASSERT_EQUAL(0xAB, frame.data[1]);
    TEST_ASSERT_EQUAL(0xEF, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[7]);
}

void test_can_decode() {
    Manager can_manager;
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

    TestCANMessage message = decode<TestCANMessage>(frame);
    
    TEST_ASSERT_EQUAL(true, message.a_bool);
    TEST_ASSERT_EQUAL(0xAB, message.a_uint8);
    TEST_ASSERT_EQUAL(0xCDEF, message.a_uint16);
    TEST_ASSERT_EQUAL(0x12345678, message.a_uint32);
}

void run_core_can_tests() {
    RUN_TEST(test_can_set_status);
    RUN_TEST(test_can_begin);
    RUN_TEST(test_can_recover);
    RUN_TEST(test_can_restart);
    RUN_TEST(test_can_end);
    RUN_TEST(test_can_transmit);
    RUN_TEST(test_can_receive);
}