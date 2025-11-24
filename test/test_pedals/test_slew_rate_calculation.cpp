#include <unity.h>
#include "../lib/pedals/calculate_slew_rate_core.h"

void test_slew_calculation() {
    uint16_t slew = 0;
    Pedals::rate_of_change_calculation(2000, 1000, 5, slew);
    TEST_ASSERT_EQUAL_UINT16(200, slew); // (2000-1000)/5 = 200
}

void test_warning_detection() {
    // simulate time progression
    uint32_t now = 0;

    // previous value 0 → current 3000 = delta 3000
    // slew = 3000/5 = 600 → WARNING
    auto out = Pedals::validate_slew_rates(3000, 0, 0, 0, now);

    TEST_ASSERT_EQUAL(Pedals::SlewStatus::WARNING, out.accel_status);
}

void test_critical_detection() {
    uint32_t now = 0;

    // delta = 6000 → slew = 1200 → CRITICAL
    auto out = Pedals::validate_slew_rates(6000, 0, 0, 0, now);

    TEST_ASSERT_EQUAL(Pedals::SlewStatus::CRITICAL, out.accel_status);
}

void tests_slew_rate() {
    RUN_TEST(test_slew_calculation);
    RUN_TEST(test_warning_detection);
    RUN_TEST(test_critical_detection);   
}