#include "unity.h"
#include <cstdint>
#include "../../lib/can/provider.h"
#include "../../lib/can/service.h"

using namespace CAN;

struct TestMessage {
    uint64_t a_bool: 1;
    uint64_t padding: 7;
    uint64_t a_uint8: 8;
    uint64_t a_uint16: 16;
    uint64_t a_uint32: 32;
};

void test_can_encode() {
    TestMessage message;
    message.a_bool = true;
    message.a_uint8 = 0xAB;
    message.a_uint16 = 0xCDEF;
    message.a_uint32 = 0x12345678;

    Frame frame(0, &message);
    
    TEST_ASSERT_EQUAL(0, frame.identifier);
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
    uint8_t data[8] = { 0x01, 0xAB, 0xEF, 0xCD, 0x78, 0x56, 0x34, 0x12 };
    Frame frame(0, data);

    TestMessage* message = frame.decode<TestMessage>();
    
    TEST_ASSERT_EQUAL(true, message->a_bool);
    TEST_ASSERT_EQUAL(0xAB, message->a_uint8);
    TEST_ASSERT_EQUAL(0xCDEF, message->a_uint16);
    TEST_ASSERT_EQUAL(0x12345678, message->a_uint32);
}

void test_can_encode_and_decode() {
    TestMessage original_message;
    original_message.a_bool = true;
    original_message.a_uint8 = 0xAB;
    original_message.a_uint16 = 0xCDEF;
    original_message.a_uint32 = 0x12345678;
    Frame frame(0, &original_message);
    TestMessage* decoded_message = frame.decode<TestMessage>();

    TEST_ASSERT_EQUAL(0, frame.identifier);
    TEST_ASSERT_EQUAL(8, frame.data_length_code);
    TEST_ASSERT_EQUAL(original_message.a_bool, decoded_message->a_bool);
    TEST_ASSERT_EQUAL(original_message.a_uint8, decoded_message->a_uint8);
    TEST_ASSERT_EQUAL(original_message.a_uint16, decoded_message->a_uint16);
    TEST_ASSERT_EQUAL(original_message.a_uint32, decoded_message->a_uint32);
}

void run_can_coding_tests() {
    RUN_TEST(test_can_encode);
    RUN_TEST(test_can_decode);
    RUN_TEST(test_can_encode_and_decode);
}