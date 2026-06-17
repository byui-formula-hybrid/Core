#ifndef BATTERY_DECODE_H
#define BATTERY_DECODE_H

#include "can/types.h"
#include <cstdint>


/**
 * ID: 0x001
 * @name Pack Instantaneous Voltage and Current
 * @param packVoltage: Voltage in V
 * @param packCurrent: Current in A
 * @param packAmpHours: Amp hours in Ah
 **/


 /**
  * @param frame: CAN frame containing the voltage message. The voltage message is expected to be in data[0] and data[1] of the frame.
  * @returns The decoded voltage value in volts. The voltage value is expected to be a 16-bit unsigned integer, where the first byte (data[0]) is the high byte and the second byte (data[1]) is the low byte. The voltage value is calculated as (data[0] << 8) | data[1].
  **/
uint16_t decodeVoltageMessage(const CAN::Frame& frame);
 



#endif // BATTERY_DECODE_H