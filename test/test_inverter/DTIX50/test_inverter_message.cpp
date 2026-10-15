#include "types.h"
#include <unity.h>
#include <cstring>

#include <inverter/DTIX50.h>
#include <core_can.h>

using namespace Inverter;
using namespace CAN;

// The inverter expects the information in BIG endian, so we use ByteSwap to convert to BigEndian so that it's easier to read what is expected

void test_message1F_encode_decode() {
    Message1F message_in = { ControlMode::CONTROL_MODE_SPEED, CAN::ByteSwap16(0x03B6), CAN::ByteSwap16(0x0167), false, 0x7FFFFF };

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message1F>();

    TEST_ASSERT_EQUAL(ControlMode::CONTROL_MODE_SPEED, message_out->getControlMode());
    TEST_ASSERT_EQUAL(950, message_out->getTargetIq());
    TEST_ASSERT_EQUAL(359, message_out->getMotorPosition());
    TEST_ASSERT_EQUAL(false, message_out->isMotorStill());
    TEST_ASSERT_EQUAL(0x7FFFFF, message_out->reserved);
}

void test_message20_encode_decode() {
    Message20 message_in = { CAN::ByteSwap32(0x01234567), CAN::ByteSwap16(-100), CAN::ByteSwap16(0x6543) };

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message20>();

    TEST_ASSERT_EQUAL(0x01234567, message_out->getERPM());
    TEST_ASSERT_EQUAL(-100, message_out->getDutyCycle());
    TEST_ASSERT_EQUAL(0x6543, message_out->getInputVoltage());
}

void test_message21_encode_decode() {
    Message21 message_in = { CAN::ByteSwap16(0x6543), CAN::ByteSwap16(0x6758), 0xFFFFFFFF };

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message21>();

    TEST_ASSERT_EQUAL(0x6543, message_out->getACCurrent());
    TEST_ASSERT_EQUAL(0x6758, message_out->getDCCurrent());
    TEST_ASSERT_EQUAL(0xFFFFFFFF, message_out->reserved);
}

void test_message22_encode_decode() {
    Message22 message_in = { CAN::ByteSwap16(0x0032), CAN::ByteSwap16(0x0102), FaultCodes::NONE, 0xFFFFFF };

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message22>();

    TEST_ASSERT_EQUAL(0x0032, message_out->getControllerTemp());
    TEST_ASSERT_EQUAL(0x0102, message_out->getMotorTemp());
    TEST_ASSERT_EQUAL(FaultCodes::NONE, message_out->getFaultCode());
    TEST_ASSERT_EQUAL(0xFFFFFF, message_out->reserved);
}

void test_message23_encode_decode() {
    Message23 message_in = { CAN::ByteSwap32(0x12345678), CAN::ByteSwap32(0x98765432) };

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message23>();

    TEST_ASSERT_EQUAL(0x12345678, message_out->getID());
    TEST_ASSERT_EQUAL(0x98765432, message_out->getIQ());
}

void test_message24_encode_decode() {
    Message24 message_in = { 0x36, 0x90, 0x6, 0x7, 0x01/* drive enable*/, 0x1, 0x1, 0x0, 0x1, 0x0, 0x0, 0x1, 0x1 /**/, 0x0, 0x0, 0x1, 0x00, 0xFF, 0x19};

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message24>();

    TEST_ASSERT_EQUAL(0x36, message_out->getThrottle());
    TEST_ASSERT_EQUAL(0x90, message_out->getBrake());
    TEST_ASSERT_EQUAL(0x6, message_out->getDigitalInputs());
    TEST_ASSERT_EQUAL(0x0, message_out->getInputPins().pin1);
    TEST_ASSERT_EQUAL(0x1, message_out->getInputPins().pin2);
    TEST_ASSERT_EQUAL(0x1, message_out->getInputPins().pin3);
    TEST_ASSERT_EQUAL(0x0, message_out->getInputPins().pin4);
    TEST_ASSERT_EQUAL(0x7, message_out->getDigitalOutputs());
    TEST_ASSERT_EQUAL(0x1, message_out->getOutputPins().pin1);
    TEST_ASSERT_EQUAL(0x1, message_out->getOutputPins().pin2);
    TEST_ASSERT_EQUAL(0x1, message_out->getOutputPins().pin3);
    TEST_ASSERT_EQUAL(0x0, message_out->getOutputPins().pin4);
    TEST_ASSERT_EQUAL(0x01, message_out->getDriveEnable());
    TEST_ASSERT_EQUAL(0x1, message_out->isCapacitorTempLimited());
    TEST_ASSERT_EQUAL(0x1, message_out->isDCCurrentLimited());
    TEST_ASSERT_EQUAL(0x0, message_out->isDriveEnableLimited());
    TEST_ASSERT_EQUAL(0x1, message_out->isIGBTAccelLimited());
    TEST_ASSERT_EQUAL(0x0, message_out->isIGBTTempLimited());
    TEST_ASSERT_EQUAL(0x0, message_out->isInputVoltageLimited());
    TEST_ASSERT_EQUAL(0x1, message_out->isMotorAccelTempLimited());
    TEST_ASSERT_EQUAL(0x1, message_out->isMotorTempLimited());
    TEST_ASSERT_EQUAL(0x0, message_out->hasRPMMin());
    TEST_ASSERT_EQUAL(0x0, message_out->hasRPMMax());
    TEST_ASSERT_EQUAL(0x1, message_out->hasPowerLimit());
    TEST_ASSERT_EQUAL(0x0, message_out->reserved0);
    TEST_ASSERT_EQUAL(0xFF, message_out->reservedFF);
    TEST_ASSERT_EQUAL(0x19, message_out->getCANMapVersion());
}

void test_message25_encode_decode() {
    Message25 message_in = { CAN::ByteSwap16(0x4030), CAN::ByteSwap16(0x1030), CAN::ByteSwap16(0x0103), CAN::ByteSwap16(0x0105) };

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message25>();

    TEST_ASSERT_EQUAL(0x4030, message_out->getMaxACCurrent());
    TEST_ASSERT_EQUAL(0x1030, message_out->getAvailableMaxACCurrent());
    TEST_ASSERT_EQUAL(0x0103, message_out->getMinACCurrent());
    TEST_ASSERT_EQUAL(0x0105, message_out->getAvailableMinACCurrent());
}

void test_message26_encode_decode() {
    Message26 message_in = { CAN::ByteSwap16(0x0503), CAN::ByteSwap16(0x0320), CAN::ByteSwap16(0x0100), CAN::ByteSwap16(0x00EF) };

    Frame frame(0x22, &message_in);

    auto message_out = frame.decode<Message26>();

    TEST_ASSERT_EQUAL(0x0503, message_out->getMaxDCCurrent());
    TEST_ASSERT_EQUAL(0x0320, message_out->getAvailableMaxDCCurrent());
    TEST_ASSERT_EQUAL(0x0100, message_out->getMinDCCurrent());
    TEST_ASSERT_EQUAL(0x00EF, message_out->getAvailableMinDCCurrent());
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
