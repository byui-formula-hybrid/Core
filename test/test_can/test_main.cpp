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

// Reuse the module main pattern and call all can tests
int main() {
    UNITY_BEGIN();
    run_manager_can_tests();
    return UNITY_END();
}