#include <unity.h>

void test_can_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_can_basic);
    return UNITY_END();
}