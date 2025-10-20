#include "unity.h"
#include "../../lib/core/serialization/binary/encode/binary_unkeyed_encoding_container.h"

using namespace Core;

// void test_initialization() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(0, container.currentIndex());
//     TEST_ASSERT_FALSE(container.isAtEnd());
//     TEST_ASSERT_EQUAL(0, container.count());
//     for (uint8_t i = 0; i < container.count(); ++i) {
//         TEST_ASSERT_EQUAL(0, container.data()[i]);
//     }
// }

// // TODO: Ensure that encoding a bool always and only ever takes up 1 bit in the buffer
// void test_encode_bool() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(true));
//     TEST_ASSERT_EQUAL(1, container.currentIndex());
//     TEST_ASSERT_TRUE(container.data()[0] == 0x01);
// }

// // TODO: Ensure that encoding a uint8 always and only ever takes up at max 8 bits in the buffer and if less than 8 bits are used, the remaining bits are ignored
// void test_encode_uint8() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt8(255)); // 11111111
//     TEST_ASSERT_EQUAL(1, container.currentIndex());
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x01));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x02));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x04));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x08));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x10));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x20));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x40));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x80));
//     TEST_ASSERT_EQUAL(255, container.data()[0]);
// }

// void test_encode_uint8_partial() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt8(5)); // 00000101
//     TEST_ASSERT_EQUAL(1, container.currentIndex());
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x01));
//     TEST_ASSERT_EQUAL(0, (container.data()[0] & 0x02));
//     TEST_ASSERT_EQUAL(1, (container.data()[0] & 0x04));
//     TEST_ASSERT_EQUAL(0, (container.data()[0] & 0x08));
//     TEST_ASSERT_EQUAL(0, (container.data()[0] & 0x10));
//     TEST_ASSERT_EQUAL(0, (container.data()[0] & 0x20));
//     TEST_ASSERT_EQUAL(0, (container.data()[0] & 0x40));
//     TEST_ASSERT_EQUAL(0, (container.data()[0] & 0x80));
//     TEST_ASSERT_EQUAL(5, container.data()[0]);
// }

// void test_encode_uint8_back_to_back() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt8(170)); // 10101010
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt8(8));  // 00001000
//     TEST_ASSERT_EQUAL(1, container.currentIndex());
//     TEST_ASSERT_EQUAL(170, container.data()[0]);
//     TEST_ASSERT_EQUAL(8, container.data()[1]);
// }

// // TODO: Ensure that encoding a uint16 always and only ever takes up at max 16 bits in the buffer and if less than 16 bits are used, the remaining bits are ignored
// void test_encode_uint16() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt16(0xABCD));
//     TEST_ASSERT_EQUAL(2, container.currentIndex());
//     TEST_ASSERT_EQUAL(0xCD, container.data()[0]);
//     TEST_ASSERT_EQUAL(0xAB, container.data()[1]);
// }

// // TODO: Ensure that encoding a uint32 always and only ever takes up at max 32 bits in the buffer and if less than 32 bits are used, the remaining bits are ignored
// void test_encode_uint32() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt32(0x12345678));
//     TEST_ASSERT_EQUAL(4, container.currentIndex());
//     TEST_ASSERT_EQUAL(0x78, container.data()[0]);
//     TEST_ASSERT_EQUAL(0x56, container.data()[1]);
//     TEST_ASSERT_EQUAL(0x34, container.data()[2]);
//     TEST_ASSERT_EQUAL(0x12, container.data()[3]);
// }

// // TODO: Ensure that encoding a uint64 always and only ever takes up at max 64 bits in the buffer and if less than 64 bits are used, the remaining bits are ignored
// void test_encode_uint64() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeUInt64(0x1122334455667788));
//     TEST_ASSERT_EQUAL(8, container.currentIndex());
//     TEST_ASSERT_EQUAL(0x88, container.data()[0]);
//     TEST_ASSERT_EQUAL(0x77, container.data()[1]);
//     TEST_ASSERT_EQUAL(0x66, container.data()[2]);
//     TEST_ASSERT_EQUAL(0x55, container.data()[3]);
//     TEST_ASSERT_EQUAL(0x44, container.data()[4]);
//     TEST_ASSERT_EQUAL(0x33, container.data()[5]);
//     TEST_ASSERT_EQUAL(0x22, container.data()[6]);
//     TEST_ASSERT_EQUAL(0x11, container.data()[7]);
// }

// // Do nothing, we don't store strings in the buffer
// void test_encode_string() {
//     BinaryUnkeyedEncodingContainer container;
//     auto dataBefore = container.data();
//     TEST_ASSERT_EQUAL(EncodeError::NotSupported, container.encodeString("Hi"));
//     TEST_ASSERT_EQUAL(0, container.currentIndex());
//     for (uint8_t i = 0; i < container.count(); ++i) {
//         TEST_ASSERT_EQUAL(0, container.data()[i]);
//     }
// }

// void test_bit_packing() {
//     BinaryUnkeyedEncodingContainer container;
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(true));
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(false));
//     TEST_ASSERT_EQUAL(EncodeError::None, container.encodeBool(true));
//     TEST_ASSERT_EQUAL(0, container.currentIndex());
//     TEST_ASSERT_EQUAL(0x03, container.data()[0]);
// }

// // TODO: Add more tests ensuring properly packing multiple values into the buffer for multiple types of values
// // TODO: Add test that validate the encoding happens within a reasonable time frame

// void run_binary_unkeyed_encoding_container_tests() {
//     RUN_TEST(test_initialization);
//     RUN_TEST(test_encode_bool);
//     RUN_TEST(test_encode_uint8);
//     RUN_TEST(test_encode_uint16);
//     RUN_TEST(test_encode_uint32);
//     RUN_TEST(test_encode_uint64);
//     RUN_TEST(test_encode_string);
//     RUN_TEST(test_bit_packing);
// }

void run_binary_unkeyed_encoding_container_tests() {
    // Placeholder for future tests
}