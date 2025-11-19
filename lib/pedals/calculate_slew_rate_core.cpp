#include "calculate_slew_rate_core.h"
#include <stdlib.h>

using namespace Pedals;

static const uint16_t CRITICAL_THRESHOLD = 1000; // delta count/ms
static const uint16_t WARNING_THRESHOLD  = 500;

static const uint32_t CRITICAL_WINDOW_MS = 50;
static const uint32_t WARNING_WINDOW_MS  = 100;

// Internal trackers
static uint32_t last_accel_critical_time = 0;
static uint32_t last_accel_warning_time  = 0;
static uint32_t last_brake_critical_time = 0;
static uint32_t last_brake_warning_time  = 0;


// -------- NUMERICAL DERIVATIVE  -------- //
void Pedals::rate_of_change_calculation(
    uint16_t current,
    uint16_t previous,
    uint16_t dt_ms,
    uint16_t &out_slew
) {
    if (dt_ms == 0) {
        out_slew = 0;
        return;
    }

    int32_t delta = static_cast<int32_t>(current) - static_cast<int32_t>(previous);
    out_slew = static_cast<uint16_t>(abs(delta) / dt_ms);
}



// -------- SLEW VALIDATION LOGIC -------- //
SlewResult Pedals::evaluate_pedal_slew(
    uint16_t accel_current,
    uint16_t accel_previous,
    uint16_t brake_current,
    uint16_t brake_previous,
    uint32_t now_ms
) {
    uint16_t accel_slew = 0;
    uint16_t brake_slew = 0;

    // 5 ms sampling
    const uint16_t DT_MS = 5;

    rate_of_change_calculation(accel_current, accel_previous, DT_MS, accel_slew);
    rate_of_change_calculation(brake_current, brake_previous, DT_MS, brake_slew);

    SlewResult result;
    result.accel_status = SlewStatus::OK;
    result.brake_status = SlewStatus::OK;



    // -------- Accelerator checks --------
    if (accel_slew > CRITICAL_THRESHOLD) {
        last_accel_critical_time = now_ms;
    } else if (accel_slew > WARNING_THRESHOLD) {
        last_accel_warning_time = now_ms;
    }

    // -------- Brake checks --------
    if (brake_slew > CRITICAL_THRESHOLD) {
        last_brake_critical_time = now_ms;
    } else if (brake_slew > WARNING_THRESHOLD) {
        last_brake_warning_time = now_ms;
    }



    // -------- TIME WINDOW EVALUATION --------
    // Accelerator
    if (now_ms - last_accel_critical_time <= CRITICAL_WINDOW_MS) {
        result.accel_status = SlewStatus::CRITICAL;
    } else if (now_ms - last_accel_warning_time <= WARNING_WINDOW_MS) {
        result.accel_status = SlewStatus::WARNING;
    }

    // Brake
    if (now_ms - last_brake_critical_time <= CRITICAL_WINDOW_MS) {
        result.brake_status = SlewStatus::CRITICAL;
    } else if (now_ms - last_brake_warning_time <= WARNING_WINDOW_MS) {
        result.brake_status = SlewStatus::WARNING;
    }

    return result;
}
