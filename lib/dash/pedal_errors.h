#pragma once

#include <cstdint>

namespace Dash {
namespace PedalErrors {

inline const char* decode(uint8_t code) {
    static constexpr const char* kNoError = "";
    static constexpr const char* kUnknown = "Unknown";
    static constexpr const char* kErrorTable[16] = {
        "",                         // 0: No error
        "Pot 1 Open Circuit",       // 1
        "Pot 1 Short To Ground",    // 2
        "Pot 1 Short To Power",     // 3
        "Pot 2 Open Circuit",       // 4
        "Pot 2 Short To Ground",    // 5
        "Pot 2 Short To Power",     // 6
        "Slew Rate Critical",       // 7
        "Pedal Implausible",        // 8
        "Brake Implausible",        // 9
        "Sensor Mismatch",          // 10
        "Calibration Fault",        // 11
        "ADC Fault",                // 12
        "Power Supply Fault",       // 13
        "Processor Fault",          // 14
        "Configuration Fault"       // 15
    };

    if (code < 16U) {
        return kErrorTable[code];
    }

    return kUnknown;
}

}  // namespace PedalErrors
}  // namespace Dash
