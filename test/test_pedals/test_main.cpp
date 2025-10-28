#include <unity.h>
#include "../../lib/pedals/core.h"

void test_pedals_basic() {
    TEST_ASSERT_TRUE(true);
}

void test_calculate_rate_change() {
    unsigned int result = 0;
    Pedals::rate_of_change_calculation(result,5,10,2);
    TEST_ASSERT_EQUAL_INT(0, result);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_pedals_basic);
    RUN_TEST(test_calculate_rate_change);
    return UNITY_END();
}