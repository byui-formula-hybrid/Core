#pragma once
#include <stdint.h>

namespace Pedals {

enum class SlewStatus {
    OK = 0,
    WARNING = 1,
    CRITICAL = 2
};

struct SlewResult {
    SlewStatus accel_status;
    SlewStatus brake_status;
};

struct SlewOutputs {
    uint16_t accel_slew;
    uint16_t brake_slew;

    SlewStatus accel_status;
    SlewStatus brake_status;
};

void rate_of_change_calculation(
    uint16_t current,
    uint16_t previous,
    uint16_t dt_ms,
    uint16_t &out_slew
);

SlewResult evaluate_pedal_slew(
    uint16_t accel_current,
    uint16_t accel_previous,
    uint16_t brake_current,
    uint16_t brake_previous,
    uint32_t now_ms       // system time in ms
);

SlewOutputs validate_slew_rates(
    uint16_t accel_current,
    uint16_t accel_previous,
    uint16_t brake_current,
    uint16_t brake_previous,
    uint32_t now_ms
);

}
