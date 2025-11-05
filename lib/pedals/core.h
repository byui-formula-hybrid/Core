#pragma once
#include <cstdint>

namespace Pedals {

enum Status {
    OK = 0,
    WARNING = 1,
    CRITICAL = 2
};

/**
 * @brief Calculates average of N readings.
 */
unsigned int calculate_average(const unsigned int* readings, unsigned int size);

/**
 * @brief Calculates slew rate (rate of change) between two averaged readings.
 */
void rate_of_change_calculation(unsigned int& out, unsigned int a, unsigned int b, unsigned int t_ms);

/**
 * @brief Evaluates slew rate against warning/critical thresholds.
 */
Status evaluate_pedal_slew_rate(unsigned int rate_of_change,
                                unsigned int threshold_warning,
                                unsigned int threshold_critical);

}