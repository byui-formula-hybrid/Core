#ifndef DTIX50_TYPES_H
#define DTIX50_TYPES_H

#include <cstdint>

namespace Inverter {
/**
    * @brief This is used for input and output pins on the inverter
    */
struct IOPins {
    bool pin1: 1;
    bool pin2: 1;
    bool pin3: 1;
    bool pin4: 1;

    explicit constexpr IOPins(uint8_t raw) :
        pin1(raw & 0x1),
        pin2(raw & 0x2),
        pin3(raw & 0x4),
        pin4(raw & 0x8)
    {}
};
// Wish I could check for just 4 bits
static_assert(sizeof(IOPins) == 0x1, "DTIX50::IOPins should be 1 byte");

// Quick compile-time test to verify that our pins map correctly
constexpr IOPins test(0b1010);
static_assert(!test.pin1, "pin1 did not map correctly");
static_assert( test.pin2, "pin2 did not map correctly");
static_assert(!test.pin3, "pin3 did not map correctly");
static_assert( test.pin4, "pin4 did not map correctly");
}

#endif
