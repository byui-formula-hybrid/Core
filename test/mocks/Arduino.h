// Arduino Mock Library for Testing
// This file provides basic Arduino function mocks for native testing

#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H

#include <stdint.h>
#include <stdio.h>

// Arduino constants
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

// Arduino data types
typedef uint8_t byte;
typedef bool boolean;

// Mock Serial class
class MockSerial {
public:
    void begin(unsigned long baud) {
        printf("Serial.begin(%lu)\n", baud);
    }
    
    void print(const char* str) {
        printf("%s", str);
    }
    
    void print(int num) {
        printf("%d", num);
    }
    
    void print(float num) {
        printf("%.2f", num);
    }
    
    void println(const char* str) {
        printf("%s\n", str);
    }
    
    void println(int num) {
        printf("%d\n", num);
    }
    
    void println(float num) {
        printf("%.2f\n", num);
    }
    
    void println() {
        printf("\n");
    }
};

// Global Serial object
extern MockSerial Serial;

// Arduino functions
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t value);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int value);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);
unsigned long millis();
unsigned long micros();

// Math functions
long map(long value, long fromLow, long fromHigh, long toLow, long toHigh);
int random(int max);
int random(int min, int max);
void randomSeed(unsigned int seed);

#endif // ARDUINO_MOCK_H