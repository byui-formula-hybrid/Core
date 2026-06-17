#include "test_main.h"
#include <battery.h>
#include <can/types.h>


void test_decode_Voltage_Message() {
    CAN::Frame frame;
    frame.data[0] = 0x12; // High byte
    frame.data[1] = 0x34; // Low byte

    TEST_ASSERT_EQUAL(0x1234, decodeVoltageMessage(frame));
}


void run_decode_Voltage_Message_tests() {
    RUN_TEST(test_decode_Voltage_Message);
}