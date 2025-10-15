#include <unity.h>

// Include test files
#include "./test_main.h"

// Setup function called before each test
void setUp(void) {
    // No platform-specific setup needed for pure reactive components
}

// Teardown function called after each test
void tearDown(void) {
    // Clean up any test artifacts
}

int main() {
    UNITY_BEGIN();

    run_publisher_tests();
    
    return UNITY_END();
}