#ifndef POT_CONVERTER_H
#define POT_CONVERTER_H

#include <Arduino.h>

// Converts an analog reading (0–4095) to a percentage (0.0–100.0)
double convertPotReading(int reading);

// Smooths out small fluctuations in readings (simple moving average)
double smoothReading(double newValue);

#endif