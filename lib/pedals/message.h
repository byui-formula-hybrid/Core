#pragma once

namespace Pedals {

struct Message {
    unsigned char accelerator_percentage: 7;
    bool is_braking: 1;
    unsigned short accelerator_potentiometer1: 12;
    unsigned short accelerator_potentiometer2: 12;
    unsigned short brake_potentiometer1: 12;
    unsigned short brake_potentiometer2: 12;
    unsigned char brake_status: 3;
    unsigned char accelerator_status: 3;
    unsigned char reserved: 2;
};

enum class Status: unsigned short {
    OK = 0,
    HARDWARE_FAILURE = 1,
    SLEW_WARNING = 2,
    SLEW_CRITICAL = 3,
    CALIBRATION_WARNING = 4,
    CALIBRATION_CRITICAL = 5
};

}