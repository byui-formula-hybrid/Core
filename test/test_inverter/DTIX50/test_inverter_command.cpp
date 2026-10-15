#include "lib/inverter/DTIX50/types.h"
#include <unity.h>
#include <cstring>

#include <inverter/DTIX50.h>
#include <core_can.h>

using namespace Inverter::Command;
using namespace CAN;

void test_SetACCurrent_command() {
    SetACCurrent command(0x0064);

    Frame frame(0x01, &command);

    // Should get 0x03E8 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0x03, frame.data[0]);
    TEST_ASSERT_EQUAL(0xE8, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetBrakeCurrent_command() {
    SetBrakeCurrent command(0x0064);

    Frame frame(0x01, &command);

    // Should get 0x03E8 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0x03, frame.data[0]);
    TEST_ASSERT_EQUAL(0xE8, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetSpeed_command() {
    SetSpeed command(0x0064);

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x00, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0x00, frame.data[2]);
    TEST_ASSERT_EQUAL(0x64, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetPosition_command() {
    SetPosition command(0x0167);

    Frame frame(0x01, &command);

    // Should get 0x0E06 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0x0E, frame.data[0]);
    TEST_ASSERT_EQUAL(0x06, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetRelativeACCurrent_command() {
    SetRelativeACCurrent command(0x0064);

    Frame frame(0x01, &command);

    // Should get 0x03E8 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0x03, frame.data[0]);
    TEST_ASSERT_EQUAL(0xE8, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetRelativeBrakeCurrent_command() {
    SetRelativeBrakeCurrent command(0x0064);

    Frame frame(0x01, &command);

    // Should get 0x03E8 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0x03, frame.data[0]);
    TEST_ASSERT_EQUAL(0xE8, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetDigitalOutput_command() {
    Inverter::IOPins pins(0b1010);
    SetDigitalOutput command(pins);

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x00, frame.data[0]); // Pin 1 Off
    TEST_ASSERT_EQUAL(0x01, frame.data[1]); // Pin 2 On
    TEST_ASSERT_EQUAL(0x00, frame.data[2]); // Pin 3 Off
    TEST_ASSERT_EQUAL(0x01, frame.data[3]); // Pin 4 On
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxACCurrent_command() {
    SetMaxACCurrent command(0x0064);

    Frame frame(0x01, &command);

    // Should get 0x03E8 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0x03, frame.data[0]);
    TEST_ASSERT_EQUAL(0xE8, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxBrakeCurrent_command() {
    SetMaxBrakeCurrent command(0x0064);

    Frame frame(0x01, &command);

    // Should get 0x03E8 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0x03, frame.data[0]);
    TEST_ASSERT_EQUAL(0xE8, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxDCCurrent_command() {
    SetMaxDCCurrent command(-100);

    Frame frame(0x01, &command);

    // Should get 0xFC18 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0xFC, frame.data[0]);
    TEST_ASSERT_EQUAL(0x18, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxBrakeDCCurrent_command() {
    SetMaxBrakeDCCurrent command(-100);

    Frame frame(0x01, &command);

    // Should get 0x03E8 since it is multiplied by 10 and in big endian
    TEST_ASSERT_EQUAL(0xFC, frame.data[0]);
    TEST_ASSERT_EQUAL(0x18, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetDriveEnable_command() {
    SetDriveEnable command(0x01);

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x01, frame.data[0]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void run_DTIX50_command_tests() {
    RUN_TEST(test_SetACCurrent_command);
    RUN_TEST(test_SetBrakeCurrent_command);
    RUN_TEST(test_SetSpeed_command);
    RUN_TEST(test_SetPosition_command);
    RUN_TEST(test_SetRelativeACCurrent_command);
    RUN_TEST(test_SetRelativeBrakeCurrent_command);
    RUN_TEST(test_SetDigitalOutput_command);
    RUN_TEST(test_SetMaxACCurrent_command);
    RUN_TEST(test_SetMaxBrakeCurrent_command);
    RUN_TEST(test_SetMaxDCCurrent_command);
    RUN_TEST(test_SetMaxBrakeDCCurrent_command);
    RUN_TEST(test_SetDriveEnable_command);
}
