#ifndef BATTERY_MESSAGES_H
#define BATTERY_MESSAGES_H

#include <cstdint>

/**
 * ID: 0x001
 * @name Pack Instantaneous Voltage and Current
 * @param packVoltage: Voltage in mV
 * @param packCurrent: Current in mA
 * @param packAmpHours: Amp hours in mAh
 **/ 
struct Message1{
    uint64_t packVoltage: 16;
    uint64_t packCurrent: 16;
    uint64_t packAmpHours: 16;
    uint64_t reserved: 16;
};

/**
 * ID: 0x002
 * @name Failsafe & DTC Flags
 * @param dtcFlags1: DTC (Diagnostic Trouble Codes) flags 1
 * @param dtcFlags2: DTC (Diagnostic Trouble Codes) flags 2
 * @param currentLimit: Current limit in mA
 */
struct Message2
{
    uint64_t dtcFlags1: 16;
    uint64_t dtcFlags2: 16;
    uint64_t currentLimit: 8;
    uint64_t reserved: 24;
};

/**
 * ID: 0x003
 * @name  MPE State
 * @param mpeState: MPE (Multi-Purpose Enable) Programable output backed
 *                  by a watchdog which will trigger on certain fault codes
 *                  (Reference Orion BMS 2 operation Manual pg:49)
 */
struct Message3{
    uint64_t mpeState: 8;
    uint64_t reserved: 56;
};

/**
 * ID: 0x004
 * @name High and Low Cell Voltages
 * @param highCellVoltage: Voltage in mV
 * @param lowCellVoltage:  Voltage in mV
 */
struct Message4{
    uint64_t highCellVoltage: 8;
    uint64_t lowCellVoltage: 8;
    uint64_t reserved: 48;
};

/**
 * ID: 0x005
 * @name Constant Value
 * @param constantValue: User-defined fixed value
 */
struct Message5 {
    uint64_t constantValue : 8;
    uint64_t reserved : 56;
};

/**
 * ID: 0x006
 * @name Pack Charge/Discharge Limits
 * @param packDCL: Discharge Current Limit in Amps
 * @param packCCL: Charge Current Limit in Amps
 * @param packCurrent: Instantaneous pack current
 * @param avgCurrent: Averaged pack current
 */
struct Message6 {
    uint64_t packDCL : 8;
    uint64_t packCCL : 8;
    uint64_t packCurrent : 16;
    uint64_t avgCurrent : 16;
    uint64_t reserved : 16;
};

/**
 * ID: 0x202
 * @name Dynamic Charge Limits
 * @param packDCL: Pack Discharge Current Limit
 * @param packCCL: Pack Charge Current Limit
 */
struct Message202 {
    uint64_t packDCL : 8;
    uint64_t packCCL : 8;
    uint64_t reserved : 48;
};

/**
 * ID: 0x351
 * @name Maximum and Minimum Pack Voltage
 * @param maxPackVoltage: Maximum allowed pack voltage
 * @param packDCL: Pack Discharge Current Limit
 * @param minPackVoltage: Minimum allowed pack voltage
 */
struct Message351 {
    uint64_t maxPackVoltage : 16;
    uint64_t packDCL : 8;
    uint64_t minPackVoltage : 16;
    uint64_t reserved : 24;
};

/**
 * ID: 0x355
 * @name Pack SOC, Health, and Temperature
 * @param packSOC: State of Charge (0-100%)
 * @param packHealth: State of Health (0-100%)
 */
struct Message355 {
    uint64_t packSOC : 8;
    uint64_t packHealth : 8;
    uint64_t reserved : 48;
};

/**
 * ID: 0x1806E7F4
 * @name Maximum Pack Voltage + Custom Flag
 * @param maxPackVoltage: Maximum pack voltage in mV
 * @param customFlag: User defined flag
 */
struct Message1806E7F4 {
    uint64_t maxPackVoltage : 16;
    uint64_t customFlag : 8;
    uint64_t reserved : 40;
};

// TODO: This message is sepcific for something but is also a copy of 1806E7F4.
// TODO: This message details needs to be modified if these two messages are for specific cell sets
/**
 * ID: 0x1806E5F4
 * @name Maximum Cell Voltage + Custom Flag
 * @param maxCellVoltage: Maximum cell voltage in mV
 * @param customFlag: User defined flag
 */
struct Message1806E5F4 {
    uint64_t maxCellVoltage : 16;
    uint64_t customFlag : 8;
    uint64_t reserved : 40;
};

// TODO: Look up the specific details of this message and modify the struct accordingly. 
// TODO: This is currently a copy of 1806E7F4, but it may have different parameters if it's for specific cell sets.
/**
 * ID: 0x1806E9F4
 * @name Maximum Cell Voltage + Custom Flag
 * @param maxCellVoltage: Maximum cell voltage in mV
 * @param customFlag: User defined flag
 */
struct Message1806E9F4 {
    uint64_t maxCellVoltage : 16;
    uint64_t customFlag : 8;
    uint64_t reserved : 40;
};

/**
 * ID: 0x18FF50E5
 * @name Blank Message
 * @note Used for logging presence on the bus.
 */
struct Message18FF50E5 {
    // No data parameters defined
    uint64_t reserved : 64;
};

#endif // BATTERY_MESSAGES_H