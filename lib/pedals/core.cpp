#include "core.h"

/*
* @brief Calculates the rate of change between two values
* @param out: this represents the rate of change
* @param a: this represents the first value at 0t
* @param b: this represents the value at 1t
* @param t: this represents the time interval
*/
void Pedals::change_calculation(unsigned int& out, unsigned int a, unsigned int b, unsigned int t) {
    out = (b - a) / t; 
}