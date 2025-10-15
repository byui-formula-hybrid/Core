#pragma once

#include <cstdint>

/**
 * @brief Basic test event for unit testing reactive components
 */
struct TestEvent {
    int value;
    uint32_t timestamp;
    
    TestEvent(int v = 0, uint32_t t = 0) : value(v), timestamp(t) {}
    
    bool operator==(const TestEvent& other) const {
        return value == other.value && timestamp == other.timestamp;
    }
};