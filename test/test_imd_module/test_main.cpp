#include <unity.h>

void test_imd_module_basic() {
    TEST_ASSERT_TRUE(true);
}

#ifdef ARDUINO_ARCH_NATIVE
int main() {
    UNITY_BEGIN();
    RUN_TEST(test_imd_module_basic);
    return UNITY_END();
}
#else
void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_imd_module_basic);
    UNITY_END();
}

void loop() {
    // Tests run in setup()
}
#endif