#include <unity.h>

#include "../../lib/dash/pedal_errors.h"

using namespace Dash;

void test_pedal_error_decode_code_zero() {
    TEST_ASSERT_EQUAL_STRING("", PedalErrors::decode(0));
}

void test_pedal_error_decode_codes_1_to_15() {
    static constexpr const char* expected[16] = {
        "",
        "Pot 1 Open Circuit",
        "Pot 1 Short To Ground",
        "Pot 1 Short To Power",
        "Pot 2 Open Circuit",
        "Pot 2 Short To Ground",
        "Pot 2 Short To Power",
        "Slew Rate Critical",
        "Pedal Implausible",
        "Brake Implausible",
        "Sensor Mismatch",
        "Calibration Fault",
        "ADC Fault",
        "Power Supply Fault",
        "Processor Fault",
        "Configuration Fault"
    };

    for (uint8_t code = 1; code <= 15; ++code) {
        TEST_ASSERT_EQUAL_STRING(expected[code], PedalErrors::decode(code));
    }
}

void test_pedal_error_decode_out_of_range() {
    TEST_ASSERT_EQUAL_STRING("Unknown", PedalErrors::decode(16));
    TEST_ASSERT_EQUAL_STRING("Unknown", PedalErrors::decode(255));
}

void run_dash_pedal_error_tests() {
    RUN_TEST(test_pedal_error_decode_code_zero);
    RUN_TEST(test_pedal_error_decode_codes_1_to_15);
    RUN_TEST(test_pedal_error_decode_out_of_range);
}
