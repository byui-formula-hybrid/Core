# Slew Rate Calculator

## 1-How the Calculator works

1. Collects readings at 5 ms intervals, where the buffer size of 1000 represents 5 seconds of data.
2. Every 5 ms, calculates an updated average for the most recent readings that the calculator receives.
3. Compares the new average to the previous average.
4. Calculates the slew rate: (change in the average)/(change in time)
5. Determines whether or not the potentiometer is working properly (gives WARNING if slew rate > threshold_warning; gives CRITICAL if slew rate > threshold_critical)
6. Warnings are flagged within 10 samples, or 50 ms.

## 2-core.h

This part of code defines the states of the slew rate that the car needs to read----OK, WARNING, and CRITICAL----and describes the main functions needed to calculate and regulate the slew rate.

## 3-core.cpp

This part of code implements how the slew rate calculator is supposed to run.

The code begins by stating the sample size and time interval for each sample (which is 5 ms).

Afterwards, the average is calculated between two readings and stored. After this, the slew rate is calculated so that the code can later evaluate the appropriate communication to the driver.

## 4-test_pedals.cpp

This part of code tests the slew rate values, to which the team can compare the values to real, pre-calculated values determined by the team.

The team can test the code through this program, so that the correct mathematical equations are used.

## 5-Possible Future Code

    #include <iostream>
    #include "core.h"

    int main() {
        constexpr int BUFFER_SIZE = 1000;
        constexpr unsigned int THRESHOLD_WARNING = 8;   // example
        constexpr unsigned int THRESHOLD_CRITICAL = 15; // example

        unsigned int pot_readings[BUFFER_SIZE] = {0};
        unsigned int index = 0;

        unsigned int last_avg = 0;
        unsigned int current_avg = 0;
        unsigned int slew_rate = 0;
        Pedals::Status status = Pedals::OK;

        // Simulate 200 samples (1 second at 5 ms/sample)
        for (int step = 0; step < 200; step++) {
            // Example: pretend pedal is pressed faster over time
            pot_readings[index] = 100 + step * 2;
            index = (index + 1) % BUFFER_SIZE;

            current_avg = Pedals::calculate_average(pot_readings, BUFFER_SIZE);

            Pedals::rate_of_change_calculation(slew_rate, last_avg, current_avg, 5);
            status = Pedals::evaluate_pedal_slew_rate(slew_rate, THRESHOLD_WARNING, THRESHOLD_CRITICAL);

            std::cout << "Step " << step
                    << " | Avg=" << current_avg
                    << " | Slew=" << slew_rate
                    << " | Status=" << status << std::endl;

            last_avg = current_avg;
        }
        return 0;
    }
This section of code could be something to implement in the future, although it is not included in this calculator. This code includes a rolling buffer, which simulates 1000 ongoing readings and still calculates the averages every 5 ms, although this code is a more complicated and optimized way to give and store the 1000 most recent readings.