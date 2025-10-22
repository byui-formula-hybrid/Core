#include "pot_converter.h"

static const int MAX_ADC_VALUE = 4095;
static const int MIN_ADC_VALUE = 0;

static const int NUM_SAMPLES = 10;  // for smoothing
static double samples[NUM_SAMPLES];
static int index = 0;
static double total = 0;

// --- Convert raw ADC to percentage (0–100)
double convertPotReading(int reading) {
    if (reading < MIN_ADC_VALUE || reading > MAX_ADC_VALUE) {
        Serial.println("Error: ADC value out of range!");
        throw std::out_of_range("ADC reading must be between 0 and 4095");
    }

    return (reading / 4095.0) * 100.0;
}

// --- Apply moving average to smooth out bumps
double smoothReading(double newValue) {
    total -= samples[index];
    samples[index] = newValue;
    total += samples[index];
    index = (index + 1) % NUM_SAMPLES;

    return total / NUM_SAMPLES;
}