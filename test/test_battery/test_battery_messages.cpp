#include "test_main.h"
#include <battery.h>
#include <can.h>
#include <cstring>

using namespace CAN;

void test_encode_decode_message_1() {
    Message1 message_in = { 0x1234, 0x5678, 0x9ABC, 0xDEF0 };
    Frame frame(0x001, &message_in);
    // Little endian packing
    TEST_ASSERT_EQUAL(0x34, frame.data[0]);
    TEST_ASSERT_EQUAL(0x12, frame.data[1]);
    TEST_ASSERT_EQUAL(0x78, frame.data[2]);
    TEST_ASSERT_EQUAL(0x56, frame.data[3]);
    TEST_ASSERT_EQUAL(0xBC, frame.data[4]);
    TEST_ASSERT_EQUAL(0x9A, frame.data[5]);
    TEST_ASSERT_EQUAL(0xF0, frame.data[6]);
    TEST_ASSERT_EQUAL(0xDE, frame.data[7]);

    auto message_out = frame.decode<Message1>();
    TEST_ASSERT_EQUAL(0x1234, message_out->packVoltage);
    TEST_ASSERT_EQUAL(0x5678, message_out->packCurrent);
    TEST_ASSERT_EQUAL(0x9ABC, message_out->packAmpHours);
    TEST_ASSERT_EQUAL(0xDEF0, message_out->reserved);
}

void test_encode_decode_message_2() {
    Message2 message_in = { 0xAAAA, 0xBBBB, 0xCC, 0x123456 };
    Frame frame(0x002, &message_in);
    TEST_ASSERT_EQUAL(0xAA, frame.data[0]);
    TEST_ASSERT_EQUAL(0xAA, frame.data[1]);
    TEST_ASSERT_EQUAL(0xBB, frame.data[2]);
    TEST_ASSERT_EQUAL(0xBB, frame.data[3]);
    TEST_ASSERT_EQUAL(0xCC, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);

    auto message_out = frame.decode<Message2>();
    TEST_ASSERT_EQUAL(0xAAAA, message_out->dtcFlags1);
    TEST_ASSERT_EQUAL(0xBBBB, message_out->dtcFlags2);
    TEST_ASSERT_EQUAL(0xCC, message_out->currentLimit);
    TEST_ASSERT_EQUAL(0x123456, message_out->reserved);
}

void test_encode_decode_message_3() {
    Message3 message_in = { 0x5A, 0x123456789ABCDE };
    Frame frame(0x003, &message_in);
    TEST_ASSERT_EQUAL(0x5A, frame.data[0]);
    TEST_ASSERT_EQUAL(0xDE, frame.data[1]);
    TEST_ASSERT_EQUAL(0xBC, frame.data[2]);
    TEST_ASSERT_EQUAL(0x9A, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);

    auto message_out = frame.decode<Message3>();
    TEST_ASSERT_EQUAL(0x5A, message_out->mpeState);
    TEST_ASSERT_EQUAL(0x123456789ABCDE, message_out->reserved);
}

void test_encode_decode_message_4() {
    Message4 message_in = { 0xAB, 0xCD, 0x123456789ABC };
    Frame frame(0x004, &message_in);
    TEST_ASSERT_EQUAL(0xAB, frame.data[0]);
    TEST_ASSERT_EQUAL(0xCD, frame.data[1]);
    TEST_ASSERT_EQUAL(0xBC, frame.data[2]);
    TEST_ASSERT_EQUAL(0x9A, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);

    auto message_out = frame.decode<Message4>();
    TEST_ASSERT_EQUAL(0xAB, message_out->highCellVoltage);
    TEST_ASSERT_EQUAL(0xCD, message_out->lowCellVoltage);
    TEST_ASSERT_EQUAL(0x123456789ABC, message_out->reserved);
}

void test_encode_decode_message_5() {
    Message5 message_in = { 0x42, 0x123456789ABCDE };
    Frame frame(0x005, &message_in);
    TEST_ASSERT_EQUAL(0x42, frame.data[0]);
    TEST_ASSERT_EQUAL(0xDE, frame.data[1]);
    TEST_ASSERT_EQUAL(0xBC, frame.data[2]);
    TEST_ASSERT_EQUAL(0x9A, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);

    auto message_out = frame.decode<Message5>();
    TEST_ASSERT_EQUAL(0x42, message_out->constantValue);
    TEST_ASSERT_EQUAL(0x123456789ABCDE, message_out->reserved);
}

void test_encode_decode_message_6() {
    Message6 message_in = { 0x11, 0x22, 0x3344, 0x5566, 0x7788 };
    Frame frame(0x006, &message_in);
    TEST_ASSERT_EQUAL(0x11, frame.data[0]);
    TEST_ASSERT_EQUAL(0x22, frame.data[1]);
    TEST_ASSERT_EQUAL(0x44, frame.data[2]);
    TEST_ASSERT_EQUAL(0x33, frame.data[3]);
    TEST_ASSERT_EQUAL(0x66, frame.data[4]);
    TEST_ASSERT_EQUAL(0x55, frame.data[5]);
    TEST_ASSERT_EQUAL(0x88, frame.data[6]);
    TEST_ASSERT_EQUAL(0x77, frame.data[7]);

    auto message_out = frame.decode<Message6>();
    TEST_ASSERT_EQUAL(0x11, message_out->packDCL);
    TEST_ASSERT_EQUAL(0x22, message_out->packCCL);
    TEST_ASSERT_EQUAL(0x3344, message_out->packCurrent);
    TEST_ASSERT_EQUAL(0x5566, message_out->avgCurrent);
    TEST_ASSERT_EQUAL(0x7788, message_out->reserved);
}

void test_encode_decode_message_202() {
    Message202 message_in = { 0xAA, 0xBB, 0x123456789ABC };
    Frame frame(0x202, &message_in);
    TEST_ASSERT_EQUAL(0xAA, frame.data[0]);
    TEST_ASSERT_EQUAL(0xBB, frame.data[1]);
    TEST_ASSERT_EQUAL(0xBC, frame.data[2]);
    TEST_ASSERT_EQUAL(0x9A, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);

    auto message_out = frame.decode<Message202>();
    TEST_ASSERT_EQUAL(0xAA, message_out->packDCL);
    TEST_ASSERT_EQUAL(0xBB, message_out->packCCL);
    TEST_ASSERT_EQUAL(0x123456789ABC, message_out->reserved);
}

void test_encode_decode_message_351() {
    Message351 message_in = { 0x1122, 0x33, 0x4455, 0x667788 };
    Frame frame(0x351, &message_in);
    TEST_ASSERT_EQUAL(0x22, frame.data[0]);
    TEST_ASSERT_EQUAL(0x11, frame.data[1]);
    TEST_ASSERT_EQUAL(0x33, frame.data[2]);
    TEST_ASSERT_EQUAL(0x55, frame.data[3]);
    TEST_ASSERT_EQUAL(0x44, frame.data[4]);
    TEST_ASSERT_EQUAL(0x88, frame.data[5]);
    TEST_ASSERT_EQUAL(0x77, frame.data[6]);
    TEST_ASSERT_EQUAL(0x66, frame.data[7]);

    auto message_out = frame.decode<Message351>();
    TEST_ASSERT_EQUAL(0x1122, message_out->maxPackVoltage);
    TEST_ASSERT_EQUAL(0x33, message_out->packDCL);
    TEST_ASSERT_EQUAL(0x4455, message_out->minPackVoltage);
    TEST_ASSERT_EQUAL(0x667788, message_out->reserved);
}

void test_encode_decode_message_355() {
    Message355 message_in = { 0x99, 0x88, 0x123456789ABC };
    Frame frame(0x355, &message_in);
    TEST_ASSERT_EQUAL(0x99, frame.data[0]);
    TEST_ASSERT_EQUAL(0x88, frame.data[1]);
    TEST_ASSERT_EQUAL(0xBC, frame.data[2]);
    TEST_ASSERT_EQUAL(0x9A, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);

    auto message_out = frame.decode<Message355>();
    TEST_ASSERT_EQUAL(0x99, message_out->packSOC);
    TEST_ASSERT_EQUAL(0x88, message_out->packHealth);
    TEST_ASSERT_EQUAL(0x123456789ABC, message_out->reserved);
}

void test_encode_decode_message_1806E7F4() {
    Message1806E7F4 message_in = { 0xAABB, 0xCC, 0x123456789A };
    Frame frame(0x1806E7F4, &message_in);
    TEST_ASSERT_EQUAL(0xBB, frame.data[0]);
    TEST_ASSERT_EQUAL(0xAA, frame.data[1]);
    TEST_ASSERT_EQUAL(0xCC, frame.data[2]);
    TEST_ASSERT_EQUAL(0x9A, frame.data[3]);
    TEST_ASSERT_EQUAL(0x78, frame.data[4]);
    TEST_ASSERT_EQUAL(0x56, frame.data[5]);
    TEST_ASSERT_EQUAL(0x34, frame.data[6]);
    TEST_ASSERT_EQUAL(0x12, frame.data[7]);

    auto message_out = frame.decode<Message1806E7F4>();
    TEST_ASSERT_EQUAL(0xAABB, message_out->maxPackVoltage);
    TEST_ASSERT_EQUAL(0xCC, message_out->customFlag);
    TEST_ASSERT_EQUAL(0x123456789A, message_out->reserved);
}

void test_encode_decode_message_1806E5F4() {
    Message1806E5F4 message_in = { 0xBEEF, 0xDD, 0x1122334455 };
    Frame frame(0x1806E5F4, &message_in);
    TEST_ASSERT_EQUAL(0xEF, frame.data[0]);
    TEST_ASSERT_EQUAL(0xBE, frame.data[1]);
    TEST_ASSERT_EQUAL(0xDD, frame.data[2]);
    TEST_ASSERT_EQUAL(0x55, frame.data[3]);
    TEST_ASSERT_EQUAL(0x44, frame.data[4]);
    TEST_ASSERT_EQUAL(0x33, frame.data[5]);
    TEST_ASSERT_EQUAL(0x22, frame.data[6]);
    TEST_ASSERT_EQUAL(0x11, frame.data[7]);

    auto message_out = frame.decode<Message1806E5F4>();
    TEST_ASSERT_EQUAL(0xBEEF, message_out->maxCellVoltage);
    TEST_ASSERT_EQUAL(0xDD, message_out->customFlag);
    TEST_ASSERT_EQUAL(0x1122334455, message_out->reserved);
}

void test_encode_decode_message_1806E9F4() {
    Message1806E9F4 message_in = { 0xCAFE, 0xEE, 0xAABBCCDDEE };
    Frame frame(0x1806E9F4, &message_in);
    TEST_ASSERT_EQUAL(0xFE, frame.data[0]);
    TEST_ASSERT_EQUAL(0xCA, frame.data[1]);
    TEST_ASSERT_EQUAL(0xEE, frame.data[2]);
    TEST_ASSERT_EQUAL(0xEE, frame.data[3]);
    TEST_ASSERT_EQUAL(0xDD, frame.data[4]);
    TEST_ASSERT_EQUAL(0xCC, frame.data[5]);
    TEST_ASSERT_EQUAL(0xBB, frame.data[6]);
    TEST_ASSERT_EQUAL(0xAA, frame.data[7]);

    auto message_out = frame.decode<Message1806E9F4>();
    TEST_ASSERT_EQUAL(0xCAFE, message_out->maxCellVoltage);
    TEST_ASSERT_EQUAL(0xEE, message_out->customFlag);
    TEST_ASSERT_EQUAL(0xAABBCCDDEE, message_out->reserved);
}

void run_encode_decode_message_18FF50E5() {
    Message18FF50E5 message_in = { 0x1122334455667788 };
    Frame frame(0x18FF50E5, &message_in);
    TEST_ASSERT_EQUAL(0x88, frame.data[0]);
    TEST_ASSERT_EQUAL(0x77, frame.data[1]);
    TEST_ASSERT_EQUAL(0x66, frame.data[2]);
    TEST_ASSERT_EQUAL(0x55, frame.data[3]);
    TEST_ASSERT_EQUAL(0x44, frame.data[4]);
    TEST_ASSERT_EQUAL(0x33, frame.data[5]);
    TEST_ASSERT_EQUAL(0x22, frame.data[6]);
    TEST_ASSERT_EQUAL(0x11, frame.data[7]);

    auto message_out = frame.decode<Message18FF50E5>();
    TEST_ASSERT_EQUAL(0x1122334455667788, message_out->reserved);
}

void run_message_tests() {
    RUN_TEST(test_encode_decode_message_1);
    RUN_TEST(test_encode_decode_message_2);
    RUN_TEST(test_encode_decode_message_3);
    RUN_TEST(test_encode_decode_message_4);
    RUN_TEST(test_encode_decode_message_5);
    RUN_TEST(test_encode_decode_message_6);
    RUN_TEST(test_encode_decode_message_202);
    RUN_TEST(test_encode_decode_message_351);
    RUN_TEST(test_encode_decode_message_355);
    RUN_TEST(test_encode_decode_message_1806E7F4);
    RUN_TEST(test_encode_decode_message_1806E5F4);
    RUN_TEST(test_encode_decode_message_1806E9F4);
    RUN_TEST(run_encode_decode_message_18FF50E5);
}