#include <unity.h>
#include "test_main.h"

void setup() {
    // Initialize any test setup
}

void teardown() {
    // Clean up any test artifacts
}

int main() {
    UNITY_BEGIN();
    run_pot_converter_tests();
    return UNITY_END();
}