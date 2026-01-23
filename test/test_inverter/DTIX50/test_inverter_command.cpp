#include <unity.h>
#include <cstring>
#include <../../lib/inverter/DTIX50/commands.h>
#include "../../lib/can/service.h"

using namespace Inverter::Command;
using namespace CAN;

void test_SetACCurrent_command() {
    SetACCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetBrakeCurrent_command() {
    SetBrakeCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetSpeed_command() {
    SetSpeed command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0x00, frame.data[2]);
    TEST_ASSERT_EQUAL(0x00, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetPosition_command() {
    SetPosition command = { 0x0167, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x67, frame.data[0]);
    TEST_ASSERT_EQUAL(0x01, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetRelativeACCurrent_command() {
    SetRelativeACCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetRelativeBrakeCurrent_command() {
    SetRelativeBrakeCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetDigitalOutput_command() {
    SetDigitalOutput command = { 0x00, 0x01, 0x00, 0x01, 0xFFFFFFFF};

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x00, frame.data[0]);
    TEST_ASSERT_EQUAL(0x01, frame.data[1]);
    TEST_ASSERT_EQUAL(0x00, frame.data[2]);
    TEST_ASSERT_EQUAL(0x01, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxACCurrent_command() {
    SetMaxACCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxBrakeCurrent_command() {
    SetMaxBrakeCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxDCCurrent_command() {
    SetMaxDCCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetMaxBrakeDCCurrent_command() {
    SetMaxBrakeDCCurrent command = { 0x0064, 0xFFFFFFFFFFFF };

    Frame frame(0x01, &command);

    TEST_ASSERT_EQUAL(0x64, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[2]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);
}

void test_SetDriveEnable_command() {
    SetDriveEnable command = { 0x01, 0xFFFFFFFFFFFFFF };

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