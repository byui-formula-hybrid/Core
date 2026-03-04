#pragma once

#include <cstdint>

/**
 * @file lib/dash/messages.h
 * Dashboard Display Module — CAN Message Definitions
 * 
 * Defines CAN frame structures for the dashboard controller to receive
 * battery, motor, pedal, and system status updates from other vehicle modules.
 * 
 * All messages use uint64_t bitfield layout for consistency with inverter messages.
 * Inbound messages only; dashboard does not transmit anything on CAN.
 */

namespace Dash {

/**
 * CAN ID constants — all inbound IDs the dashboard listens to
 */
namespace CANID {
    constexpr uint32_t AMSVoltCurrent = 0x101;
    constexpr uint32_t AMSTemperature = 0x102;
    constexpr uint32_t AMSFault = 0x103;
    constexpr uint32_t IMDFault = 0x104;
    constexpr uint32_t MotorStatus = 0x201;
    constexpr uint32_t VCUStatus = 0x301;
    constexpr uint32_t PedalData = 0x401;
    constexpr uint32_t Heartbeat = 0x3FF;
}  // namespace CANID

/**
 * ID: 0x101
 * @name Battery Pack Voltage & Current
 * @param pack_voltage_raw: Battery pack voltage in 0.1 V units (scale: *0.1 to get volts).
 *                          Example: 4200 raw = 420.0 V
 * @param pack_current_raw: Battery pack current in 0.1 A units (scale: *0.1 to get amps).
 *                          Positive = discharging, negative = charging.
 * @param reserved: For future use.
 */
struct AMSVoltCurrent {
    uint64_t pack_voltage_raw : 16;   // [0.1 V / LSB]
    uint64_t pack_current_raw : 16;   // [0.1 A / LSB, signed]
    uint64_t reserved : 32;
};

/**
 * ID: 0x102
 * @name Battery Pack Temperature
 * @param max_cell_temp_raw: Maximum cell temperature in 0.1 °C units.
 *                           Example: 450 raw = 45.0 °C
 * @param reserved: For future use.
 */
struct AMSTemperature {
    uint64_t max_cell_temp_raw : 16;  // [0.1 °C / LSB]
    uint64_t reserved : 48;
};

/**
 * ID: 0x103  
 * @name Battery Management System Fault Status
 * @param over_voltage: 1 = pack voltage > max threshold
 * @param under_voltage: 1 = pack voltage < min threshold
 * @param over_temp: 1 = a cell exceeds max temperature
 * @param under_temp: 1 = a cell falls below min temperature
 * @param cell_imbalance: 1 = cell voltage spread exceeds threshold
 * @param crc_error: 1 = CRC check failed on internal communication
 * @param hardware_error: 1 = BMS hardware fault (internal failure)
 * @param reserved: For future use.
 */
struct AMSFault {
    uint64_t over_voltage : 1;
    uint64_t under_voltage : 1;
    uint64_t over_temp : 1;
    uint64_t under_temp : 1;
    uint64_t cell_imbalance : 1;
    uint64_t crc_error : 1;
    uint64_t hardware_error : 1;
    uint64_t reserved : 57;
};

/**
 * ID: 0x104
 * @name Insulation Monitoring Device Fault Status
 * @param iso_fault: 1 = insulation resistance below threshold (critical for driver safety)
 * @param reserved: For future use.
 */
struct IMDFault {
    uint64_t iso_fault : 1;
    uint64_t reserved : 63;
};

/**
 * ID: 0x201
 * @name Motor/Inverter Status
 * @param motor_rpm_raw: Motor electrical RPM. 
 *                       Directly maps to inverter eRPM field.
 * @param reserved: For future use.
 */
struct MotorStatus {
    uint64_t motor_rpm_raw : 32;      // [RPM], unsigned
    uint64_t reserved : 32;
};

/**
 * ID: 0x301
 * @name Vehicle Control Unit Status
 * @param ts_active: 1 = Traction System is armed and ready, 0 = TS disabled
 * @param glv_on: 1 = GLV (General Low-Voltage) battery active
 * @param ready_to_drive: 1 = RTD buzzer active / car ready to drive
 * @param reserved: For future use.
 */
struct VCUStatus {
    uint64_t ts_active : 1;           // Tractive System Active
    uint64_t glv_on : 1;               // GLV online
    uint64_t ready_to_drive : 1;       // RTD signal
    uint64_t reserved : 61;
};

/**
 * ID: 0x401
 * @name Pedal & Sensor Input Data
 * @param throttle_pct_raw: Throttle pedal position in percent [0-100].
 * @param brake_pressed: 1 = brake pedal pressed, 0 = not pressed
 * @param pedal_adc1_raw: Raw ADC value from throttle pot 1
 * @param pedal_adc2_raw: Raw ADC value from throttle pot 2
 * @param pedal_error_code: 0 = no error. [1-15] = specific fault code.
 *                          See Dash::PedalErrors::decode() for interpretation.
 * @param pedal_sync_fault: 1 = the two pots do not agree (implausibility fault)
 */
struct PedalData {
    uint64_t throttle_pct_raw : 8;    // [0-100%]
    uint64_t brake_pressed : 1;        // bool
    uint64_t reserved1 : 7;            // alignment
    uint64_t pedal_adc1_raw : 16;     // 12-bit ADC, typically 0-4095
    uint64_t pedal_adc2_raw : 16;     // 12-bit ADC, typically 0-4095
    uint64_t pedal_error_code : 4;    // [0-15] fault code
    uint64_t pedal_sync_fault : 1;     // Implausibility
    uint64_t reserved2 : 11;           // alignment to 64 bits
};

/**
 * ID: 0x3FF
 * @name Heartbeat (System Alive)
 * @param counter: Increments each heartbeat; used to detect CAN loss and message frequency.
 * @param reserved: For future use.
 */
struct Heartbeat {
    uint64_t counter : 8;              // Rolls over 0-255
    uint64_t reserved : 56;
};

}  // namespace Dash
