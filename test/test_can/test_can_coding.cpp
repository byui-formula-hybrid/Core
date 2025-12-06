#include "unity.h"
#include <cstdint>
#include "../../lib/can/manager.h"
#include "../../lib/can/core.h"

using namespace CAN;

struct TestMessage {
    uint64_t a_bool: 1;
    uint64_t padding: 7;
    uint64_t a_uint8: 8;
    uint64_t a_uint16: 16;
    uint64_t a_uint32: 32;
};

void test_can_encode() {
    Message<TestMessage> message;
    message.data = new TestMessage();
    message.data->a_bool = true;
    message.data->a_uint8 = 0xAB;
    message.data->a_uint16 = 0xCDEF;
    message.data->a_uint32 = 0x12345678;
    message.id = 1;

    Frame frame = encode(message);
    
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

void test_can_encode_and_decode() {
    Message<TestMessage> original_message;
    original_message.data = new TestMessage();
    original_message.data->a_bool = true;
    original_message.data->a_uint8 = 0xAB;
    original_message.data->a_uint16 = 0xCDEF;
    original_message.data->a_uint32 = 0x12345678;
    original_message.id = 1;
    Frame frame = encode(original_message);
    Message<TestMessage> decoded_message = decode<TestMessage>(&frame);

    TEST_ASSERT_EQUAL(1, decoded_message.id);
    TEST_ASSERT_EQUAL(original_message.data->a_bool, decoded_message.data->a_bool);
    TEST_ASSERT_EQUAL(original_message.data->a_uint8, decoded_message.data->a_uint8);
    TEST_ASSERT_EQUAL(original_message.data->a_uint16, decoded_message.data->a_uint16);
    TEST_ASSERT_EQUAL(original_message.data->a_uint32, decoded_message.data->a_uint32);
}

void run_can_coding_tests() {
    RUN_TEST(test_can_encode);
    RUN_TEST(test_can_decode);
    RUN_TEST(test_can_encode_and_decode);
}