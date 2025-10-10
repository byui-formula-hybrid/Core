#include <unity.h>

void test_dash_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_dash_basic);
    return UNITY_END();
}