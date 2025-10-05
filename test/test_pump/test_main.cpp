#include <unity.h>

void test_pump_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_pump_basic);
    return UNITY_END();
}