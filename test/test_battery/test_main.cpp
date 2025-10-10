#include <unity.h>

void test_battery_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_battery_basic);
    return UNITY_END();
}