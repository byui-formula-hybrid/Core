#pragma once

/**
 * @brief Battery-related event for testing battery module reactive components
 */
struct BatteryEvent {
    float voltage;
    float current;
    float temperature;
    
    BatteryEvent(float v = 0.0f, float c = 0.0f, float t = 0.0f) 
        : voltage(v), current(c), temperature(t) {}
    
    bool operator==(const BatteryEvent& other) const {
        return voltage == other.voltage && 
               current == other.current && 
               temperature == other.temperature;
    }
};