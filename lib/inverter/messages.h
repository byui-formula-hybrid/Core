#pragma once

#include <cstdint>

// Derived from
// https://zapdrive.eu/docs/assets/common/can_docs/v25/DTI%20CAN%20manual%20V2.5.pdf

namespace Inverter {

enum class ControlMode : uint64_t {
    // UNUSED MODES 0, 5, 6
    CONTROL_MODE_SPEED = 1,
    CONTROL_MODE_CURRENT = 2,
    CONTROL_MODE_CURRENT_BRAKE = 3,
    CONTROL_MODE_POSITION = 4,
    CONTROL_MODE_NONE = 7
};

/*******************************
 * @param OVERVOLTAGE:  The input voltage is higher than the set
 *                      maximum
 * @param UNDERVOLTAGE: The input voltage is lower than the set
 *                      minimum
 * @param DRV: Transistor or transistor drive error
 * @param ABS_OVERCURRENT: The AC current is higher than the set
 *                         absolute maximum current.
 * @param CTRL_OVERTEMP: The controller temperature is higher than
 *                       the set maximum
 * @param MOTOR_OVERTEMP: The motor temperature is higher than the
 *                        set maximum
 * @param SENSOR_WIRE: Something went wrong with the sensor
 *                     differential signals
 * @param SENSOR: An error occurred while processing the
 *                sensor signals
 * @param CAN_COMMAND: CAN message received contains
 *                     parameter out of boundaries
 * @param ANALOG_INPUT: Redundant output out of range
 ******************************/
enum class FaultCodes : uint8_t {
    NONE = 0x00,
    OVERVOLTAGE = 0x01,
    UNDERVOLTAGE = 0x02,
    DRV = 0x03,
    ABS_OVERCURRENT = 0x04,
    CTRL_OVERTEMP = 0x05,
    MOTOR_OVERTEMP = 0x06,
    SENSOR_WIRE = 0x07,
    SENSOR = 0x08,
    CAN_COMMAND = 0x09,
    ANALOG_INPUT = 0x0A,
};

/**
 * ID: 0x1F
 * @name General data 6
 * @param control_mode: Describes the control mode of the inverter - Mainly used in multicontroller configuration 
 *               for internal communications.
 * @param target_iq: The value represent how much Iq current the inverter is targeted to reach. 
 *            This value excludes limits. For ex. if the target Iq is 50 A and temperature limit is hit, 
 *            the values keep 50 A in any case, not including the deration of the temp. limit. 
 *            This is useful in multi-inverter configuration to let know the secondary inverter the target Iq. 
 * @param motor_position: Motor position expressed in degrees. 
 * @param is_motor_still: Represents if the motor in still position or not. 1: still, 0: rotating 
 * @param reserved: Filled with FF’s. For future use. 
 */
struct Message1F {
    ControlMode control_mode: 8;
    uint64_t target_iq: 16;
    uint64_t motor_position: 16;
    uint64_t is_motor_still: 1;
    uint64_t reserved: 23;
};

/*******************************
 * ID: 0x20
 * @name General data 1
 * @param erpm: Electrical RPM
 * @param duty_cycle: The controller duty cycle. The sign of this value will represent whether
 *                   the motor is running(positive) current or regenerating (negative)
 *                   current.
 * @param input_voltage: Input voltage is the DC voltage.
 ******************************/
struct Message20 {
    uint64_t erpm : 32;
    uint64_t duty_cycle : 16;
    uint64_t input_voltage : 16;
};

/*******************************
 * ID: 0x21
 * @name General data 2
 * @param ac_current: The motor current. The sign of this value represents whether the motor
 *                    is running(positive) current or regenerating (negative) current
 * @param dc_current: DC Current: Current on DC side. The sign of this value represents
 *                    whether the motor is running(positive) current or regenerating
 *                    (negative) current.
 * @param reserved: Filled with 0xFFs
 ******************************/
struct Message21 {
    uint64_t ac_current : 16;
    uint64_t dc_current : 16;
    uint64_t reserved : 32;
};

/*******************************
 * ID: 0x22
 * @name General data 3
 * @param controller_temp: Temperature of the inverter semiconductors.
 * @param motor_temp: Temperature of the motor measured by the inverter 
 * @param fault_code: Defined with the FaultCodes Enum
 * @param reserved: Filled with 0xFFs
 ******************************/
struct Message22 {
    uint64_t controller_temp : 16;
    uint64_t motor_temp : 16;
    FaultCodes fault_code : 8;
    uint64_t reserved : 24;
};

/*******************************
 * ID: 0x23
 * General data 4
 * @param id: FOC algorithm component Id
 * @param iq: FOC algorithm component Iq 
 ******************************/
struct Message23 {
    uint64_t id : 32;
    uint64_t iq : 32;
};

/*******************************
 * ID: 0x24
 * @name General data 5
 * @param throttle_signal: Throttle signal derived from analog inputs or CAN2
 * @param brake_signal: Brake signal derived from analog inputs or CAN2
 * @param digital_inputs: Digital input pins 1-4, 1 is active and 0 is inactive
 * @param digital_outputs: Digital output pins 1-4, 1 is active and 0 is inactive
 * @param drive_enable: 1 is Drive Enabled and 0 is Drive Disabled. Drive can be enabled/disbled by the digital input or/and via CAN2
 *                     interface
 * @param capacitor_temp_limit: The inverter can limit the output power to not to overheat the
 *                            internal capacitors. (only valid HW version 3.6 or newer). 1 is Limit active, 0 is limit inactive
 * @param dc_current_limit: 1 is Limit active, 0 is limit inactive
 * @param drive_enable_limit: Indicates whether the drive enable limitation is active or inactive.
 *                          Used for software development purposes. Refer to DriveEnable for true indication
 * @param igbt_accel_limit: 1 is limit active, 0 is limit inactive
 * @param igbt_temp_limit: 1 is limit active, 0 is limit inactive
 * @param input_voltage_limit: 1 is limit active, 0 is limit inactive
 * @param motor_accel_temp_limit: 1 is limit active, 0 is limit inactive
 * @param motor_temp_limit: 1 is limit active, 0 is limit inactive
 * @param rpm_min_limit: 1 is limit active, 0 is limit inactive
 * @param rpm_max_limit: 1 is limit active, 0 is limit inactive
 * @param power_limit: 1 is limit by configuration active, 0 is limit by configuration inactive
 * @param reserved0: Set to 0
 * @param reservedFF: Filled with 0xFFs
 * @param can_map_version: Indicates the CAN map version. For ex: 25 -> 2,5 (V2,5)
 ******************************/
struct Message24 {
    uint64_t throttle_signal : 8;
    uint64_t brake_signal : 8;
    uint64_t digital_inputs : 4;
    uint64_t digital_outputs : 4;
    uint64_t drive_enable : 8;
    uint64_t capacitor_temp_limit : 1;
    uint64_t dc_current_limit : 1;
    uint64_t drive_enable_limit : 1;
    uint64_t igbt_accel_limit : 1;
    uint64_t igbt_temp_limit : 1;
    uint64_t input_voltage_limit : 1;
    uint64_t motor_accel_temp_limit : 1;
    uint64_t motor_temp_limit : 1;
    uint64_t rpm_min_limit : 1;
    uint64_t rpm_max_limit : 1;
    uint64_t power_limit : 1;
    uint64_t reserved0 : 4;
    uint64_t reservedFF : 8;
    uint64_t can_map_version : 8;
};

/*******************************
 * ID: 0x25
 * @name Configured and available AC currents 
 * @param max_ac_current: Max AC current configured with the DTI CAN tool. 
 * @param av_max_ac_current: Available max AC current. This value affected by the limitation
 *                        functions (igbt temp, motortemp etc.) Defines how much current
 *                        available from the configured one.
 * @param min_ac_current: Min AC current configured with the DTI CAN tool. 
 * @param av_min_ac_current: Available min AC current. This value affected by the limitation
 *                        functions (igbt temp, motortemp etc.) Defines how much current
 *                        available from the configured one.
 ******************************/
struct Message25 {
   uint64_t max_ac_current : 16;
   uint64_t av_max_ac_current : 16;
   uint64_t min_ac_current : 16;
   uint64_t av_min_ac_current : 16;
};

/*******************************
 * ID: 0x26
 * @name Configured and available DC currents
 * @param max_dc_current: Max DC current configured with the DTI CAN tool. 
 * @param av_max_dc_current: Available max DC current. This value affected by the limitation
 *                        functions (igbt temp, motortemp etc.) Defines how much current
 *                        available from the configured one.
 * @param min_dc_current: Min DC current configured with the DTI CAN tool. 
 * @param av_min_dc_current: Available min DC current. This value affected by the limitation
 *                        functions (igbt temp, motortemp etc.) Defines how much current
 *                        available from the configured one.
 ******************************/
struct Message26 {
   uint64_t max_dc_current : 16;
   uint64_t av_max_dc_current : 16;
   uint64_t min_dc_current : 16;
   uint64_t av_min_dc_current : 16;
};

};