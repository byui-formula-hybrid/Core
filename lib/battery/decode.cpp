#include "decode.h"

using namespace CAN;

uint16_t decodeVoltageMessage(const CAN::Frame& frame) {
    // Implementation for decoding voltage message
    uint16_t voltage = ((uint16_t)frame.data[0] << 8) | frame.data[1];
    return voltage;
}
