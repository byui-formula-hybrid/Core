#include <unity.h>
#include "../../lib/core/serialization/data.h"

void test_data_initialization() {
    Core::Data dataBuffer(16);
    TEST_ASSERT_EQUAL(0, dataBuffer.size(size));

    TEST_ASSERT_EQUAL(16, dataBuffer.capacity());

    uint8_t output[16] = {0};
    std::copy_n(dataBuffer.bytes(), 16, output);
    for (std::size_t i = 0; i < 16; ++i) {
        TEST_ASSERT_EQUAL(0, output[i]);
    }
}

void test_data_append_and_read() {
    Core::Data dataBuffer(16);
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.append(0xABCD1234));

    TEST_ASSERT_EQUAL(4, dataBuffer.size());

    uint64_t value = 0;
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.read(value, 32));
    TEST_ASSERT_EQUAL(0xABCD1234, value);
}

void test_data_clear() {
    Core::Data dataBuffer(16);
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.append(0x12345678));

    TEST_ASSERT_EQUAL(4, dataBuffer.size());

    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.clear());

    TEST_ASSERT_EQUAL(0, dataBuffer.size());
}

void test_data_copy_constructor() {
    Core::Data originalBuffer(16);
    TEST_ASSERT_EQUAL(Core::Data::Error::None, originalBuffer.append(0xDEADBEEF));

    Core::Data copiedBuffer = originalBuffer;

    TEST_ASSERT_EQUAL(4, copiedBuffer.size());

    uint64_t value = 0;
    TEST_ASSERT_EQUAL(Core::Data::Error::None, copiedBuffer.read(value, 32));
    TEST_ASSERT_EQUAL(0xDEADBEEF, value);
}

void test_data_assignment_operator() {
    Core::Data originalBuffer(16);
    TEST_ASSERT_EQUAL(Core::Data::Error::None, originalBuffer.append(0xFEEDC0DE));

    Core::Data assignedBuffer(8);
    assignedBuffer = originalBuffer;

    TEST_ASSERT_EQUAL(4, assignedBuffer.size());

    uint64_t value = 0;
    TEST_ASSERT_EQUAL(Core::Data::Error::None, assignedBuffer.read(value, 32));
    TEST_ASSERT_EQUAL(0xFEEDC0DE, value);
}

void test_data_capacity() {
    Core::Data dataBuffer(32);
    TEST_ASSERT_EQUAL(32, dataBuffer.capacity());
}

void test_data_size() {
    Core::Data dataBuffer(16);
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.append(0x1234));

    TEST_ASSERT_EQUAL(2, dataBuffer.size());
}

void test_data_bytes() {
    Core::Data dataBuffer(16);
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.append(0xAABBCCDD));

    uint8_t output[4] = {0};
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.bytes(output, 4));
    TEST_ASSERT_EQUAL(0xDD, output[0]);
    TEST_ASSERT_EQUAL(0xCC, output[1]);
    TEST_ASSERT_EQUAL(0xBB, output[2]);
    TEST_ASSERT_EQUAL(0xAA, output[3]);
}

void test_data_bit_packing() {
    Core::Data dataBuffer(16);
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.append(0b10101010)); // 8 bits
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.append(0b1100));     // 4 bits

    std::size_t size = 0;
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.size(size));
    TEST_ASSERT_EQUAL(2, size); // Should occupy 2 bytes

    uint64_t value1 = 0;
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.read(value1, 8));
    TEST_ASSERT_EQUAL(0b10101010, value1);

    uint64_t value2 = 0;
    TEST_ASSERT_EQUAL(Core::Data::Error::None, dataBuffer.read(value2, 4));
    TEST_ASSERT_EQUAL(0b1100, value2);
}

void run_data_tests() {
    RUN_TEST(test_data_initialization);
    RUN_TEST(test_data_append_and_read);
    RUN_TEST(test_data_clear);
    RUN_TEST(test_data_copy_constructor);
    RUN_TEST(test_data_assignment_operator);
    RUN_TEST(test_data_capacity);
    RUN_TEST(test_data_size);
    RUN_TEST(test_data_bytes);
    RUN_TEST(test_data_bit_packing);
}