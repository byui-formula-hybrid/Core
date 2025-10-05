// Arduino Mock Library Implementation for Testing

#include "Arduino.h"
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <thread>

// Global Serial object
MockSerial Serial;

// Pin state storage for mocking
static uint8_t pinModes[40] = {0};
static uint8_t digitalPinStates[40] = {0};
static int analogPinValues[20] = {0};

// Initialize random seed
static bool randomInitialized = false;

void pinMode(uint8_t pin, uint8_t mode) {
    if (pin < 40) {
        pinModes[pin] = mode;
        printf("pinMode(%d, %s)\n", pin, 
               mode == OUTPUT ? "OUTPUT" : 
               mode == INPUT ? "INPUT" : "INPUT_PULLUP");
    }
}

void digitalWrite(uint8_t pin, uint8_t value) {
    if (pin < 40) {
        digitalPinStates[pin] = value;
        printf("digitalWrite(%d, %s)\n", pin, value ? "HIGH" : "LOW");
    }
}

int digitalRead(uint8_t pin) {
    if (pin < 40) {
        printf("digitalRead(%d) = %s\n", pin, digitalPinStates[pin] ? "HIGH" : "LOW");
        return digitalPinStates[pin];
    }
    return LOW;
}

int analogRead(uint8_t pin) {
    if (pin < 20) {
        // Return mock value or previously set value
        int value = analogPinValues[pin] > 0 ? analogPinValues[pin] : 512; // Default mid-range
        printf("analogRead(%d) = %d\n", pin, value);
        return value;
    }
    return 0;
}

void analogWrite(uint8_t pin, int value) {
    if (pin < 40) {
        printf("analogWrite(%d, %d)\n", pin, value);
    }
}

void delay(unsigned long ms) {
    printf("delay(%lu ms)\n", ms);
    // Actually delay for testing timing
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void delayMicroseconds(unsigned int us) {
    printf("delayMicroseconds(%u us)\n", us);
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}

unsigned long millis() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
    return duration.count();
}

unsigned long micros() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
    return duration.count();
}

long map(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

int random(int max) {
    if (!randomInitialized) {
        srand(time(nullptr));
        randomInitialized = true;
    }
    return rand() % max;
}

int random(int min, int max) {
    if (!randomInitialized) {
        srand(time(nullptr));
        randomInitialized = true;
    }
    return min + (rand() % (max - min));
}

void randomSeed(unsigned int seed) {
    srand(seed);
    randomInitialized = true;
}

// Mock functions for setting test values
namespace ArduinoMock {
    void setAnalogValue(uint8_t pin, int value) {
        if (pin < 20) {
            analogPinValues[pin] = value;
        }
    }
    
    void setDigitalValue(uint8_t pin, uint8_t value) {
        if (pin < 40) {
            digitalPinStates[pin] = value;
        }
    }
    
    void reset() {
        for (int i = 0; i < 40; i++) {
            pinModes[i] = 0;
            digitalPinStates[i] = 0;
        }
        for (int i = 0; i < 20; i++) {
            analogPinValues[i] = 0;
        }
    }
}