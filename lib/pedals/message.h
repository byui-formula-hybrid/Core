#pragma once

#include <cstdint>

namespace Pedals {

/* 
 * Message structure for pedal data communication.
 * Uses bit-fields to optimize data size.
 * Total size: 8 bytes (64 bits)
 * - accelerator_percentage: 7 bits
 * - is_braking: 1 bit
 * - accelerator_potentiometer1: 12 bits
 * - accelerator_potentiometer2: 12 bits
 * - brake_potentiometer1: 12 bits
 * - brake_potentiometer2: 12 bits
 * - brake_status: 3 bits
 * - accelerator_status: 3 bits
 * - reserved: 2 bits
 */
struct Message {
    uint64_t accelerator_percentage: 7;
    uint64_t is_braking: 1;
    uint64_t accelerator_potentiometer1: 12;
    uint64_t accelerator_potentiometer2: 12;
    uint64_t brake_potentiometer1: 12;
    uint64_t brake_potentiometer2: 12;
    uint64_t brake_status: 3;
    uint64_t accelerator_status: 3;
    uint64_t reserved: 2;
};

/* 
 * Status codes for pedal system.
 */
enum class Status: uint64_t {
    OK = 0,
    HARDWARE_FAILURE = 1,
    SLEW_WARNING = 2,
    SLEW_CRITICAL = 3,
    CALIBRATION_WARNING = 4,
    CALIBRATION_CRITICAL = 5
};

}