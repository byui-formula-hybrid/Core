#include "test_main.h"
#include "unity.h"

#include <cstdint>
#include <core_can.h>

void test_swapbyte16() {
    uint16_t first = CAN::ByteSwap16(0x0100);
    uint16_t max = CAN::ByteSwap16(0xFFFF);
    uint16_t min = CAN::ByteSwap16(0);
    uint16_t alternate = CAN::ByteSwap16(0x0110);
    uint16_t swap_back = CAN::ByteSwap16(0xFF00);

    TEST_ASSERT_EQUAL(0x0001, first);
    TEST_ASSERT_EQUAL(0xFFFF, max);
    TEST_ASSERT_EQUAL(0, min);
    TEST_ASSERT_EQUAL(0x1001, alternate);
    TEST_ASSERT_EQUAL(0x00FF, swap_back);
    TEST_ASSERT_EQUAL(0xFF00, CAN::ByteSwap16(swap_back));
}

void test_sswapbyte16() {
    int16_t first = CAN::SByteSwap16(0x0100);
    int16_t max = CAN::SByteSwap16(0x7FFF);
    int16_t min = CAN::SByteSwap16((int16_t)0x8000);
    int16_t negative = CAN::SByteSwap16(-720);

    TEST_ASSERT_EQUAL(0x0001, first);
    TEST_ASSERT_EQUAL((int16_t)0xFF7F, max);
    TEST_ASSERT_EQUAL((int16_t)0x0080, min);
    TEST_ASSERT_EQUAL((int16_t)0x30FD, negative);
}

void test_swapbyte32() {
    uint32_t first = CAN::ByteSwap32(0x01000000);
    uint32_t max = CAN::ByteSwap32(0xFFFFFFFF);
    uint32_t min = CAN::ByteSwap32(0x00000000);
    uint32_t alternate = CAN::ByteSwap32(0x01100000);
    uint32_t swap_back = CAN::ByteSwap32(0xFF000000);

    TEST_ASSERT_EQUAL(0x00000001, first);
    TEST_ASSERT_EQUAL(0xFFFFFFFF, max);
    TEST_ASSERT_EQUAL(0x00000000, min);
    TEST_ASSERT_EQUAL(0x00001001, alternate);
    TEST_ASSERT_EQUAL(0x000000FF, swap_back);
    TEST_ASSERT_EQUAL(0xFF000000, CAN::ByteSwap32(swap_back));
}

void test_sswapbyte32() {
    int32_t first = CAN::SByteSwap32(0x01000000);
    int32_t max = CAN::SByteSwap32(0x7FFFFFFF);
    int32_t min = CAN::SByteSwap32((int32_t)0x80000000);
    int32_t negative = CAN::SByteSwap32(-720);

    TEST_ASSERT_EQUAL(0x00000001, first);
    TEST_ASSERT_EQUAL((int32_t)0xFFFFFF7F, max);
    TEST_ASSERT_EQUAL(0x00000080, min);
    TEST_ASSERT_EQUAL((int32_t)0x30FDFFFF, negative);
}


void run_byteswap_tests() {
    test_swapbyte16();
    test_sswapbyte16();
    test_swapbyte32();
    test_sswapbyte32();
}
