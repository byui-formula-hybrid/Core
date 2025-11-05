#include <unity.h>
#include "../../lib/pedals/core.h"

void test_calculate_average() {
    unsigned int data[4] = {1, 2, 3, 4};
    unsigned int avg = Pedals::calculate_average(data, 4);
    TEST_ASSERT_EQUAL_UINT(2, avg);
}

void test_slew_rate_calculation() {
    unsigned int rate = 0;
    Pedals::rate_of_change_calculation(rate, 100, 150, 5);
    TEST_ASSERT_EQUAL_UINT(10, rate); // (150-100)/5 = 10
}

void test_evaluate_slew_rate_warning() {
    unsigned int rate = 9;
    auto status = Pedals::evaluate_pedal_slew_rate(rate, 8, 15);
    TEST_ASSERT_EQUAL_INT(Pedals::WARNING, status);
}

void test_evaluate_slew_rate_critical() {
    unsigned int rate = 20;
    auto status = Pedals::evaluate_pedal_slew_rate(rate, 8, 15);
    TEST_ASSERT_EQUAL_INT(Pedals::CRITICAL, status);
}

void test_evaluate_slew_rate_ok() {
    unsigned int rate = 5;
    auto status = Pedals::evaluate_pedal_slew_rate(rate, 8, 15);
    TEST_ASSERT_EQUAL_INT(Pedals::OK, status);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_calculate_average);
    RUN_TEST(test_slew_rate_calculation);
    RUN_TEST(test_evaluate_slew_rate_ok);
    RUN_TEST(test_evaluate_slew_rate_warning);
    RUN_TEST(test_evaluate_slew_rate_critical);
    return UNITY_END();
}
