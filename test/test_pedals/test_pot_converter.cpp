#include <unity.h>
#include "../../lib/pedals/pot_converter.h"

PotConverter converter;

void test_valid_reading_conversion(void) {
    double result = converter.convertPotReading(2048);
    TEST_ASSERT_TRUE(result >= 49.9 && result <= 50.1);
}

void test_out_of_range_low(void) {
    double result = converter.convertPotReading(-10);
    TEST_ASSERT_EQUAL(-1, result);
}

void test_out_of_range_high(void) {
    double result = converter.convertPotReading(5000);
    TEST_ASSERT_EQUAL(-1, result);
}

void test_smoothing(void) {
    converter.smoothReading(10);
    converter.smoothReading(20);
    converter.smoothReading(30);
    double result = converter.smoothReading(40);
    TEST_ASSERT_TRUE(result > 10 && result < 40);
}

void run_pot_converter_tests() {
    RUN_TEST(test_valid_reading_conversion);
    RUN_TEST(test_out_of_range_low);
    RUN_TEST(test_out_of_range_high);
    RUN_TEST(test_smoothing);
}