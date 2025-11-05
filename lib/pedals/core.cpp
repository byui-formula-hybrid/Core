#include "core.h"

namespace Pedals {

// Fixed sampling interval (ms)
constexpr unsigned int SAMPLE_INTERVAL_MS = 5;

// ----------------------------------------------------
// Average calculator
// ----------------------------------------------------
unsigned int calculate_average(const unsigned int* readings, unsigned int size) {
    if (size == 0) return 0;
    unsigned long sum = 0;
    for (unsigned int i = 0; i < size; i++) {
        sum += readings[i];
    }
    return static_cast<unsigned int>(sum / size);
}

// ----------------------------------------------------
// Slew rate calculator
// ----------------------------------------------------
void rate_of_change_calculation(unsigned int& out, unsigned int a, unsigned int b, unsigned int t_ms) {
    if (t_ms == 0) {
        out = 0;
        return;
    }
    unsigned int diff = (a > b) ? (a - b) : (b - a);
    out = diff / t_ms; // units per millisecond
}

// ----------------------------------------------------
// Slew rate evaluator
// ----------------------------------------------------
Status evaluate_pedal_slew_rate(unsigned int rate_of_change,
                                unsigned int threshold_warning,
                                unsigned int threshold_critical) {
    if (rate_of_change >= threshold_critical)
        return CRITICAL;
    if (rate_of_change >= threshold_warning)
        return WARNING;
    return OK;
}

}
