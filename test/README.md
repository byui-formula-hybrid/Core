# test/ Directory

## Purpose
This directory contains **unit tests** for each module in the project.

## What Goes Here
- **Unit tests** - Test files for each module
- **Test utilities** - Helper functions for testing (future)
- **Mock objects** - Simulated hardware for testing (future)

## What Does NOT Go Here
- ❌ Main application code (that goes in `src/`)
- ❌ Module implementation (that goes in `lib/`)
- ❌ Integration tests (those would go in a separate directory)

## Structure Pattern
Each module gets its own test directory:
```
test/
├── test_module_name/
│   └── test_main.cpp          # Unit tests for ModuleName
└── test_another_module/
    └── test_main.cpp          # Unit tests for AnotherModule
```

## Current Test Modules
- **`test_can_module/`** - Tests for CanModule
- **`test_core_module/`** - Tests for CoreModule
- **`test_dash_module/`** - Tests for DashModule
- **`test_inverter_module/`** - Tests for InverterModule
- **`test_pump_module/`** - Tests for PumpModule
- **`test_pedals_module/`** - Tests for PedalsModule
- **`test_battery_module/`** - Tests for BatteryModule
- **`test_imd_module/`** - Tests for ImdModule

## Test Template
Each test file follows this pattern:

```cpp
#include <unity.h>
#include <Arduino.h>
#include "ModuleName.h"

ModuleName* module;

void setUp(void) {
    // Set up before each test
    module = new ModuleName();
}

void tearDown(void) {
    // Clean up after each test
    delete module;
    module = nullptr;
}

void test_module_initialization() {
    // Test module initialization
    TEST_ASSERT_TRUE(true); // Replace with actual test
}

void test_module_begin() {
    // Test begin() method
    TEST_ASSERT_TRUE(module->begin());
}

void test_module_update() {
    // Test update() method
    module->update();
    TEST_ASSERT_TRUE(true); // Replace with actual test
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    UNITY_BEGIN();
    
    RUN_TEST(test_module_initialization);
    RUN_TEST(test_module_begin);
    RUN_TEST(test_module_update);
    
    UNITY_END();
}

void loop() {
    // Tests run in setup()
}
```

## Running Tests
Use PlatformIO commands to run tests:

```bash
# Run all tests
pio test

# Run tests for specific module
pio test -f test_pedals_module

# Run with verbose output
pio test -v
```

## Test Writing Guidelines

### Good Tests Should:
- ✅ **Test one thing** - Each test function should test one specific behavior
- ✅ **Be independent** - Tests should not depend on other tests
- ✅ **Be repeatable** - Same test should always give same result
- ✅ **Have clear names** - Test name should describe what is being tested

### Example Test Cases to Write:
- **Initialization** - Does the module initialize correctly?
- **Valid inputs** - Does the module handle normal inputs correctly?
- **Invalid inputs** - Does the module handle errors gracefully?
- **Edge cases** - What happens at boundary conditions?
- **State transitions** - Do state changes work correctly?

## Unity Framework
We use the Unity testing framework:

- `TEST_ASSERT_TRUE(condition)` - Assert condition is true
- `TEST_ASSERT_FALSE(condition)` - Assert condition is false
- `TEST_ASSERT_EQUAL(expected, actual)` - Assert values are equal
- `TEST_ASSERT_NULL(pointer)` - Assert pointer is null
- `TEST_ASSERT_NOT_NULL(pointer)` - Assert pointer is not null

## Future Additions
- **Mock hardware** - Simulate sensors and actuators for testing
- **Test utilities** - Helper functions for common test patterns
- **Integration tests** - Test multiple modules working together