#include <unity.h>
#include <cstring>
#include <cstdio>
#include <../../lib/inverter/DTIX50/messages.h>
#include "../../lib/can/service.h"

using namespace Inverter;
using namespace CAN;

void test_message1F_encode_decode() {
    Message1F message_in = { ControlMode::CONTROL_MODE_SPEED, 0x03B6, 0x0167, false, 0x7FFFFF };

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x01, frame.data[0]);
    TEST_ASSERT_EQUAL(0xB6, frame.data[1]);
    TEST_ASSERT_EQUAL(0x03, frame.data[2]);
    TEST_ASSERT_EQUAL(0x67, frame.data[3]);
    TEST_ASSERT_EQUAL(0x01, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFE, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);

    auto message_out = frame.decode<Message1F>();

    TEST_ASSERT_EQUAL(ControlMode::CONTROL_MODE_SPEED, message_out->control_mode);
    TEST_ASSERT_EQUAL(950, message_out->target_iq);
    TEST_ASSERT_EQUAL(359, message_out->motor_position);
    TEST_ASSERT_EQUAL(false, message_out->is_motor_still);
    TEST_ASSERT_EQUAL(0x7FFFFF, message_out->reserved);
}

void test_message20_encode_decode() {
    Message20 message_in = { 0x01234567, 0x9876, 0x6543 };

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x67, frame.data[0]);
    TEST_ASSERT_EQUAL(0x45, frame.data[1]);
    TEST_ASSERT_EQUAL(0x23, frame.data[2]);
    TEST_ASSERT_EQUAL(0x01, frame.data[3]);
    TEST_ASSERT_EQUAL(0x76, frame.data[4]);
    TEST_ASSERT_EQUAL(0x98, frame.data[5]);
    TEST_ASSERT_EQUAL(0x43, frame.data[6]);
    TEST_ASSERT_EQUAL(0x65, frame.data[7]);

    auto message_out = frame.decode<Message20>();

    TEST_ASSERT_EQUAL(0x01234567, message_out->erpm);
    TEST_ASSERT_EQUAL(0x9876, message_out->duty_cycle);
    TEST_ASSERT_EQUAL(0x6543, message_out->input_voltage);
}

void test_message21_encode_decode() {
    Message21 message_in = { 0x6543, 0x6758, 0xFFFFFFFF };

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x43, frame.data[0]);
    TEST_ASSERT_EQUAL(0x65, frame.data[1]);
    TEST_ASSERT_EQUAL(0x58, frame.data[2]);
    TEST_ASSERT_EQUAL(0x67, frame.data[3]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);

    auto message_out = frame.decode<Message21>();

    TEST_ASSERT_EQUAL(0x6543, message_out->ac_current);
    TEST_ASSERT_EQUAL(0x6758, message_out->dc_current);
    TEST_ASSERT_EQUAL(0xFFFFFFFF, message_out->reserved);
}

void test_message22_encode_decode() {
    Message22 message_in = { 0x0032, 0x0102, FaultCodes::NONE, 0xFFFFFF };

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x32, frame.data[0]);
    TEST_ASSERT_EQUAL(0x00, frame.data[1]);
    TEST_ASSERT_EQUAL(0x02, frame.data[2]);
    TEST_ASSERT_EQUAL(0x01, frame.data[3]);
    TEST_ASSERT_EQUAL(0x00, frame.data[4]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[7]);

    auto message_out = frame.decode<Message22>();

    TEST_ASSERT_EQUAL(0x0032, message_out->controller_temp);
    TEST_ASSERT_EQUAL(0x0102, message_out->motor_temp);
    TEST_ASSERT_EQUAL(FaultCodes::NONE, message_out->fault_code);
    TEST_ASSERT_EQUAL(0xFFFFFF, message_out->reserved);
}

void test_message23_encode_decode() {
    Message23 message_in = { 0x12345678, 0x98765432 };

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x78, frame.data[0]);
    TEST_ASSERT_EQUAL(0x56, frame.data[1]);
    TEST_ASSERT_EQUAL(0x34, frame.data[2]);
    TEST_ASSERT_EQUAL(0x12, frame.data[3]);
    TEST_ASSERT_EQUAL(0x32, frame.data[4]);
    TEST_ASSERT_EQUAL(0x54, frame.data[5]);
    TEST_ASSERT_EQUAL(0x76, frame.data[6]);
    TEST_ASSERT_EQUAL(0x98, frame.data[7]);

    auto message_out = frame.decode<Message23>();

    TEST_ASSERT_EQUAL(0x12345678, message_out->id);
    TEST_ASSERT_EQUAL(0x98765432, message_out->iq);
}

void test_message24_encode_decode() {
    Message24 message_in = { 0x36, 0x90, 0x6, 0x7, 0x01/* drive enable*/, 0x1, 0x1, 0x0, 0x1, 0x0, 0x0, 0x1, 0x1 /**/, 0x0, 0x0, 0x1, 0x00, 0xFF, 0x19};

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x36, frame.data[0]);
    TEST_ASSERT_EQUAL(0x90, frame.data[1]);
    TEST_ASSERT_EQUAL(0x76, frame.data[2]);
    TEST_ASSERT_EQUAL(0x01, frame.data[3]);
    TEST_ASSERT_EQUAL(0xCB, frame.data[4]);
    TEST_ASSERT_EQUAL(0x04, frame.data[5]);
    TEST_ASSERT_EQUAL(0xFF, frame.data[6]);
    TEST_ASSERT_EQUAL(0x19, frame.data[7]);

    auto message_out = frame.decode<Message24>();

    TEST_ASSERT_EQUAL(0x36, message_out->throttle_signal);
    TEST_ASSERT_EQUAL(0x90, message_out->brake_signal);
    TEST_ASSERT_EQUAL(0x6, message_out->digital_inputs);
    TEST_ASSERT_EQUAL(0x7, message_out->digital_outputs);
    TEST_ASSERT_EQUAL(0x01, message_out->drive_enable);
    TEST_ASSERT_EQUAL(0x1, message_out->capacitor_temp_limit);
    TEST_ASSERT_EQUAL(0x1, message_out->dc_current_limit);
    TEST_ASSERT_EQUAL(0x0, message_out->drive_enable_limit);
    TEST_ASSERT_EQUAL(0x1, message_out->igbt_accel_limit);
    TEST_ASSERT_EQUAL(0x0, message_out->igbt_temp_limit);
    TEST_ASSERT_EQUAL(0x0, message_out->input_voltage_limit);
    TEST_ASSERT_EQUAL(0x1, message_out->motor_accel_temp_limit);
    TEST_ASSERT_EQUAL(0x1, message_out->motor_temp_limit);
    TEST_ASSERT_EQUAL(0x0, message_out->rpm_min_limit);
    TEST_ASSERT_EQUAL(0x0, message_out->rpm_max_limit);
    TEST_ASSERT_EQUAL(0x1, message_out->power_limit);
    TEST_ASSERT_EQUAL(0x0, message_out->reserved0);
    TEST_ASSERT_EQUAL(0xFF, message_out->reservedFF);
    TEST_ASSERT_EQUAL(0x19, message_out->can_map_version);
}

void test_message25_encode_decode() {
    Message25 message_in = { 0x4030, 0x1030, 0x0103, 0x0103 };

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x30, frame.data[0]);
    TEST_ASSERT_EQUAL(0x40, frame.data[1]);
    TEST_ASSERT_EQUAL(0x30, frame.data[2]);
    TEST_ASSERT_EQUAL(0x10, frame.data[3]);
    TEST_ASSERT_EQUAL(0x03, frame.data[4]);
    TEST_ASSERT_EQUAL(0x01, frame.data[5]);
    TEST_ASSERT_EQUAL(0x03, frame.data[6]);
    TEST_ASSERT_EQUAL(0x01, frame.data[7]);

    auto message_out = frame.decode<Message25>();

    TEST_ASSERT_EQUAL(0x4030, message_out->max_ac_current);
    TEST_ASSERT_EQUAL(0x1030, message_out->av_max_ac_current);
    TEST_ASSERT_EQUAL(0x0103, message_out->min_ac_current);
    TEST_ASSERT_EQUAL(0x0103, message_out->av_min_ac_current);
}

void test_message26_encode_decode() {
    Message26 message_in = { 0x0503, 0x0320, 0x0100, 0x00EF };

    Frame frame(0x22, &message_in);

    TEST_ASSERT_EQUAL(0x03, frame.data[0]);
    TEST_ASSERT_EQUAL(0x05, frame.data[1]);
    TEST_ASSERT_EQUAL(0x20, frame.data[2]);
    TEST_ASSERT_EQUAL(0x03, frame.data[3]);
    TEST_ASSERT_EQUAL(0x00, frame.data[4]);
    TEST_ASSERT_EQUAL(0x01, frame.data[5]);
    TEST_ASSERT_EQUAL(0xEF, frame.data[6]);
    TEST_ASSERT_EQUAL(0x00, frame.data[7]);

    auto message_out = frame.decode<Message26>();

    TEST_ASSERT_EQUAL(0x0503, message_out->max_dc_current);
    TEST_ASSERT_EQUAL(0x0320, message_out->av_max_dc_current);
    TEST_ASSERT_EQUAL(0x0100, message_out->min_dc_current);
    TEST_ASSERT_EQUAL(0x00EF, message_out->av_min_dc_current);
}


void run_DTIX50_message_tests() {
    RUN_TEST(test_message1F_encode_decode);
    RUN_TEST(test_message20_encode_decode);
    RUN_TEST(test_message21_encode_decode);
    RUN_TEST(test_message22_encode_decode);
    RUN_TEST(test_message23_encode_decode);
    RUN_TEST(test_message24_encode_decode);
    RUN_TEST(test_message25_encode_decode);
    RUN_TEST(test_message26_encode_decode);
}