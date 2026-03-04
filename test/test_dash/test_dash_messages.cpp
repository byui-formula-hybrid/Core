#include <unity.h>
#include <cstring>
#include "../../lib/dash/messages.h"
#include "../../lib/can/service.h"

using namespace Dash;
using namespace CAN;

// Test AMS Voltage & Current message
void test_ams_volt_current_encode_decode() {
    AMSVoltCurrent msg_in = { 4200, 500, 0 };  // 420V, 50A

    Frame frame(CANID::AMSVoltCurrent, &msg_in);

    // Verify byte layout (little-endian)
    // 4200 = 0x1068, so bytes are [0x68, 0x10, ...]
    TEST_ASSERT_EQUAL(0x68, frame.data[0]);  // 4200 & 0xFF
    TEST_ASSERT_EQUAL(0x10, frame.data[1]);  // (4200 >> 8) & 0xFF
    // 500 = 0x01F4, so bytes are [0xF4, 0x01, ...]
    TEST_ASSERT_EQUAL(0xF4, frame.data[2]);  // 500 & 0xFF
    TEST_ASSERT_EQUAL(0x01, frame.data[3]);  // (500 >> 8) & 0xFF

    auto msg_out = frame.decode<AMSVoltCurrent>();

    TEST_ASSERT_EQUAL(4200, msg_out->pack_voltage_raw);
    TEST_ASSERT_EQUAL(500, msg_out->pack_current_raw);
    TEST_ASSERT_EQUAL(0, msg_out->reserved);
}

// Test AMS Temperature message
void test_ams_temperature_encode_decode() {
    AMSTemperature msg_in = { 450, 0 };  // 45.0°C

    Frame frame(CANID::AMSTemperature, &msg_in);

    TEST_ASSERT_EQUAL(0xC2, frame.data[0]);  // 450 & 0xFF
    TEST_ASSERT_EQUAL(0x01, frame.data[1]);  // (450 >> 8) & 0xFF

    auto msg_out = frame.decode<AMSTemperature>();

    TEST_ASSERT_EQUAL(450, msg_out->max_cell_temp_raw);
    TEST_ASSERT_EQUAL(0, msg_out->reserved);
}

// Test AMS Fault message
void test_ams_fault_encode_decode() {
    AMSFault msg_in = { 1, 0, 1, 0, 0, 0, 0, 0 };  // Over-voltage and over-temp

    Frame frame(CANID::AMSFault, &msg_in);

    TEST_ASSERT_EQUAL(0x05, frame.data[0]);  // bits 0 and 2 set

    auto msg_out = frame.decode<AMSFault>();

    TEST_ASSERT_EQUAL(1, msg_out->over_voltage);
    TEST_ASSERT_EQUAL(0, msg_out->under_voltage);
    TEST_ASSERT_EQUAL(1, msg_out->over_temp);
    TEST_ASSERT_EQUAL(0, msg_out->under_temp);
}

// Test IMD Fault message
void test_imd_fault_encode_decode() {
    IMDFault msg_in = { 1, 0 };  // ISO fault detected

    Frame frame(CANID::IMDFault, &msg_in);

    TEST_ASSERT_EQUAL(0x01, frame.data[0]);

    auto msg_out = frame.decode<IMDFault>();

    TEST_ASSERT_EQUAL(1, msg_out->iso_fault);
}

// Test Motor Status message
void test_motor_status_encode_decode() {
    MotorStatus msg_in = { 5000, 0 };  // 5000 RPM

    Frame frame(CANID::MotorStatus, &msg_in);

    TEST_ASSERT_EQUAL(0x88, frame.data[0]);  // 5000 & 0xFF
    TEST_ASSERT_EQUAL(0x13, frame.data[1]);  // (5000 >> 8) & 0xFF

    auto msg_out = frame.decode<MotorStatus>();

    TEST_ASSERT_EQUAL(5000, msg_out->motor_rpm_raw);
    TEST_ASSERT_EQUAL(0, msg_out->reserved);
}

// Test VCU Status message
void test_vcu_status_encode_decode() {
    VCUStatus msg_in = { 1, 1, 0, 0 };  // TS on, GLV on, RTD off

    Frame frame(CANID::VCUStatus, &msg_in);

    TEST_ASSERT_EQUAL(0x03, frame.data[0]);  // bits 0 and 1 set

    auto msg_out = frame.decode<VCUStatus>();

    TEST_ASSERT_EQUAL(1, msg_out->ts_active);
    TEST_ASSERT_EQUAL(1, msg_out->glv_on);
    TEST_ASSERT_EQUAL(0, msg_out->ready_to_drive);
}

// Test Pedal Data message
void test_pedal_data_encode_decode() {
    PedalData msg_in = { 
        75,     // throttle 75%
        0,      // brake not pressed
        0,      // reserved1
        2048,   // ADC1 = 0x800
        1024,   // ADC2 = 0x400
        0,      // no error
        0,      // no sync fault
        0       // reserved2
    };

    Frame frame(CANID::PedalData, &msg_in);

    auto msg_out = frame.decode<PedalData>();

    TEST_ASSERT_EQUAL(75, msg_out->throttle_pct_raw);
    TEST_ASSERT_EQUAL(0, msg_out->brake_pressed);
    TEST_ASSERT_EQUAL(2048, msg_out->pedal_adc1_raw);
    TEST_ASSERT_EQUAL(1024, msg_out->pedal_adc2_raw);
    TEST_ASSERT_EQUAL(0, msg_out->pedal_error_code);
    TEST_ASSERT_EQUAL(0, msg_out->pedal_sync_fault);
}

// Test Pedal Data with error code
void test_pedal_data_with_error_encode_decode() {
    PedalData msg_in = { 
        100,    // throttle 100%
        1,      // brake pressed
        0,      // reserved1
        4095,   // ADC1 maxed
        4095,   // ADC2 maxed
        7,      // error code (Slew Rate Critical, example)
        1,      // sync fault
        0       // reserved2
    };

    Frame frame(CANID::PedalData, &msg_in);

    auto msg_out = frame.decode<PedalData>();

    TEST_ASSERT_EQUAL(100, msg_out->throttle_pct_raw);
    TEST_ASSERT_EQUAL(1, msg_out->brake_pressed);
    TEST_ASSERT_EQUAL(4095, msg_out->pedal_adc1_raw);
    TEST_ASSERT_EQUAL(4095, msg_out->pedal_adc2_raw);
    TEST_ASSERT_EQUAL(7, msg_out->pedal_error_code);
    TEST_ASSERT_EQUAL(1, msg_out->pedal_sync_fault);
}

// Test Heartbeat message
void test_heartbeat_encode_decode() {
    Heartbeat msg_in = { 42, 0 };  // Counter = 42

    Frame frame(CANID::Heartbeat, &msg_in);

    TEST_ASSERT_EQUAL(42, frame.data[0]);

    auto msg_out = frame.decode<Heartbeat>();

    TEST_ASSERT_EQUAL(42, msg_out->counter);
    TEST_ASSERT_EQUAL(0, msg_out->reserved);
}

// Test Heartbeat counter rollover
void test_heartbeat_counter_rollover() {
    Heartbeat msg_in = { 255, 0 };  // Counter at max

    Frame frame(CANID::Heartbeat, &msg_in);

    auto msg_out = frame.decode<Heartbeat>();

    TEST_ASSERT_EQUAL(255, msg_out->counter);
}

// Verify CAN IDs are correct
void test_canid_constants() {
    TEST_ASSERT_EQUAL(0x101, CANID::AMSVoltCurrent);
    TEST_ASSERT_EQUAL(0x102, CANID::AMSTemperature);
    TEST_ASSERT_EQUAL(0x103, CANID::AMSFault);
    TEST_ASSERT_EQUAL(0x104, CANID::IMDFault);
    TEST_ASSERT_EQUAL(0x201, CANID::MotorStatus);
    TEST_ASSERT_EQUAL(0x301, CANID::VCUStatus);
    TEST_ASSERT_EQUAL(0x401, CANID::PedalData);
    TEST_ASSERT_EQUAL(0x3FF, CANID::Heartbeat);
}

// Verify message sizes (all should be 8 bytes)
void test_message_sizes() {
    TEST_ASSERT_EQUAL(8, sizeof(AMSVoltCurrent));
    TEST_ASSERT_EQUAL(8, sizeof(AMSTemperature));
    TEST_ASSERT_EQUAL(8, sizeof(AMSFault));
    TEST_ASSERT_EQUAL(8, sizeof(IMDFault));
    TEST_ASSERT_EQUAL(8, sizeof(MotorStatus));
    TEST_ASSERT_EQUAL(8, sizeof(VCUStatus));
    TEST_ASSERT_EQUAL(8, sizeof(PedalData));
    TEST_ASSERT_EQUAL(8, sizeof(Heartbeat));
}
