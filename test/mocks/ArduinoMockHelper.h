// Arduino Mock Helper Functions for Testing
// This file provides additional helper functions for testing Arduino code

#ifndef ARDUINO_MOCK_HELPER_H
#define ARDUINO_MOCK_HELPER_H

#include "Arduino.h"

namespace ArduinoMock {
    // Helper functions to set mock values for testing
    void setAnalogValue(uint8_t pin, int value);
    void setDigitalValue(uint8_t pin, uint8_t value);
    void reset();  // Reset all pin states
    
    // Test verification functions
    bool wasPinModeSet(uint8_t pin, uint8_t mode);
    bool wasDigitalWriteCalled(uint8_t pin, uint8_t value);
    int getLastAnalogWrite(uint8_t pin);
}

#endif // ARDUINO_MOCK_HELPER_H