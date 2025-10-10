#include <unity.h>

void test_pedals_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_pedals_basic);
    return UNITY_END();
}