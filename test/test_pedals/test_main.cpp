#include <unity.h>
#include "test_main.h"

// Setup function called before each test
void setUp(void) {
    // No platform-specific setup needed for pure reactive components
}

// Teardown function called after each test
void tearDown(void) {
    // Clean up any test artifacts
}

void test_pedals_basic() {
    TEST_ASSERT_TRUE(true);
}

int main() {
    UNITY_BEGIN();
    tests_slew_rate();   
    return UNITY_END();
}

// .\.scripts\powershell\test.ps1 --verbose ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ THIS HELPSSSSSSS when you open powershell