#pragma once

namespace Pedals {

// enum Status {
//     OK = 0,
//     WARNING = 1,
//     CRITICAL = 2
// }

// void calculate_rate_of_change(uint16_t &rate_of_change, uint16_t* data, time_interval_t interval);

// Status evaluate_pedal_slew_rate(uint16_t* rate_of_change, uint16_t threshold_warning, uint16_t threshold_critical);

void rate_of_change_calculation(unsigned int& out, unsigned int a, unsigned int b, unsigned int t);

}