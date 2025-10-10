#include <unity.h>

void test_inverter_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_inverter_basic);
    return UNITY_END();
}