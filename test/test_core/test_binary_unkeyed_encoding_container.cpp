#include "unity.h"
#include "../../lib/core/serialization/binary/encode/binary_unkeyed_encoding_container.h"

using namespace Core;

void test_initialization() {
    BinaryUnkeyedEncodingContainer container;
    TEST_ASSERT_EQUAL(0, container.currentIndex());
    TEST_ASSERT_FALSE(container.isAtEnd());
    TEST_ASSERT_EQUAL(8, container.count());
    for (uint8_t i = 0; i < container.count(); ++i) {
        TEST_ASSERT_EQUAL(0, container.data()[i]);
    }
}

void test_encode_bool() {
    BinaryUnkeyedEncodingContainer container;
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(true));
    TEST_ASSERT_EQUAL(1, container.currentIndex());
    TEST_ASSERT_TRUE(container.data()[0] == 1);
}

void test_encode_uint8() {
    BinaryUnkeyedEncodingContainer container;
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt8(255));
    TEST_ASSERT_EQUAL(1, container.currentIndex());
    TEST_ASSERT_EQUAL(255, container.data()[0]);
}

void test_encode_uint16() {
    BinaryUnkeyedEncodingContainer container;
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt16(0xABCD));
    TEST_ASSERT_EQUAL(2, container.currentIndex());
    TEST_ASSERT_EQUAL(0xCD, container.data()[0]);
    TEST_ASSERT_EQUAL(0xAB, container.data()[1]);
}

void test_encode_uint32() {
    BinaryUnkeyedEncodingContainer container;
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt32(0x12345678));
    TEST_ASSERT_EQUAL(4, container.currentIndex());
    TEST_ASSERT_EQUAL(0x78, container.data()[0]);
    TEST_ASSERT_EQUAL(0x56, container.data()[1]);
    TEST_ASSERT_EQUAL(0x34, container.data()[2]);
    TEST_ASSERT_EQUAL(0x12, container.data()[3]);
}

void test_encode_uint64() {
    BinaryUnkeyedEncodingContainer container;
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt64(0x1122334455667788));
    TEST_ASSERT_EQUAL(8, container.currentIndex());
    TEST_ASSERT_EQUAL(0x88, container.data()[0]);
    TEST_ASSERT_EQUAL(0x77, container.data()[1]);
    TEST_ASSERT_EQUAL(0x66, container.data()[2]);
    TEST_ASSERT_EQUAL(0x55, container.data()[3]);
    TEST_ASSERT_EQUAL(0x44, container.data()[4]);
    TEST_ASSERT_EQUAL(0x33, container.data()[5]);
    TEST_ASSERT_EQUAL(0x22, container.data()[6]);
    TEST_ASSERT_EQUAL(0x11, container.data()[7]);
}

// Do nothing, we don't store strings in the buffer
void test_encode_string() {
    BinaryUnkeyedEncodingContainer container;
    auto dataBefore = container.data();
    TEST_ASSERT_EQUAL(EncodeError::NotSupported, container.encodeString("Hi"));
    TEST_ASSERT_EQUAL(0, container.currentIndex());
    TEST_ASSERT_EQUAL(dataBefore, container.data());
}

void test_bit_packing() {
    BinaryUnkeyedEncodingContainer container;
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(true));
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(false));
    TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(true));
    TEST_ASSERT_EQUAL(0, container.currentIndex());
    TEST_ASSERT_EQUAL(0x01, container.data()[0]);
    TEST_ASSERT_EQUAL(0x02, container.data()[1]);
}

// TODO: Add more tests ensuring properly packing multiple values into the buffer for multiple types of values
// TODO: Add test that validate the encoding happens within a reasonable time frame

void run_binary_unkeyed_encoding_container_tests() {
    RUN_TEST(test_initialization);
    RUN_TEST(test_encode_bool);
    RUN_TEST(test_encode_uint8);
    RUN_TEST(test_encode_uint16);
    RUN_TEST(test_encode_uint32);
    RUN_TEST(test_encode_uint64);
    RUN_TEST(test_encode_string);
    RUN_TEST(test_bit_packing);
}