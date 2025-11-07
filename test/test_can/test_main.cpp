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

void test_can_basic() {
    TEST_ASSERT_TRUE(true);
}


// Reuse the module main pattern and call all can tests
int main() {
    UNITY_BEGIN();
    
    return UNITY_END();
}