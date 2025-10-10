#include <unity.h>

void test_imd_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_imd_basic);
    return UNITY_END();
}