# Arduino Mock Library for Testing

This directory contains Arduino mock implementations that allow you to run unit tests on your local machine without requiring actual ESP32 hardware.

## Files

- `Arduino.h` - Mock Arduino header with function declarations
- `Arduino.cpp` - Mock Arduino implementation with console output
- `ArduinoMockHelper.h` - Helper functions for testing

## Usage

### Running Tests with Mocks

```bash
# Run tests on native platform (no hardware required)
pio test --environment native

# Run tests on ESP32 (requires hardware)
pio test --environment esp32dev_test --without-uploading
```

### Using Mocks in Test Code

```cpp
#ifdef ARDUINO_ARCH_NATIVE
    // Use Arduino mocks for native testing
    #include "Arduino.h"
    #include "ArduinoMockHelper.h"
#else
    // Use real Arduino for hardware testing
    #include <Arduino.h>
#endif

void test_example() {
    // Set mock analog value
    ArduinoMock::setAnalogValue(A0, 512);
    
    // Read the mocked value
    int value = analogRead(A0);
    TEST_ASSERT_EQUAL(512, value);
    
    // Test digital I/O
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    
    // Reset all mocks
    ArduinoMock::reset();
}
```

## Mock Functions

### Available Arduino Functions
- `pinMode(pin, mode)`
- `digitalWrite(pin, value)`
- `digitalRead(pin)`
- `analogRead(pin)`
- `analogWrite(pin, value)`
- `delay(ms)`
- `delayMicroseconds(us)`
- `millis()`
- `micros()`
- `Serial.begin()`, `Serial.print()`, etc.

### Mock Control Functions
- `ArduinoMock::setAnalogValue(pin, value)` - Set analog read value
- `ArduinoMock::setDigitalValue(pin, value)` - Set digital read value
- `ArduinoMock::reset()` - Reset all pin states

## Benefits

1. **Fast Testing** - No upload time, runs instantly
2. **CI/CD Ready** - Run tests in automated builds
3. **Debug Friendly** - Console output shows all Arduino calls
4. **Isolated Testing** - Each test can reset mock state

## Console Output

When running tests, you'll see Arduino function calls:
```
Serial.begin(115200)
pinMode(13, OUTPUT)
digitalWrite(13, HIGH)
analogRead(0) = 512
delay(1000 ms)
```

## Integration with VS Code

The mocks work seamlessly with VS Code's testing interface when using PlatformIO.