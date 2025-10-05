#include <unity.h>

void test_core_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_core_basic);
    return UNITY_END();
}