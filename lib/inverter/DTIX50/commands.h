#pragma once

#include <cstdint>

// Derived from
// https://zapdrive.eu/docs/assets/common/can_docs/v25/DTI%20CAN%20manual%20V2.5.pdf

namespace Inverter {
namespace Command {

/**
 * ID: 0x01
 * @param ac_current: This command sets the target motor AC current (peak, not RMS).
 *                    When the controller receives this message, it automatically
 *                    switches to current control mode.
 *                    This value must not be above the limits of the inverter and must be
 *                    multiplied by 10 before sending. This is a signed parameter, and the
 *                    sign represents the direction of the torque which correlates with the
 *                    motor AC current. (For the correlation, please refer to the motor
 *                    parameters)
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC
 **/
struct SetACCurrent {
    uint64_t ac_current : 16;
    uint64_t reserved : 48;
};

/**
 * ID: 0x02
 * @param brake_current: Targets the brake current of the motor. It will result negative torque
 *                       relatively to the forward direction of the motor.
 *                       This value must be multiplied by 10 before sending, only positive
 *                       currents are accepted
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetBrakeCurrent {
    uint64_t brake_current : 16;
    uint64_t reserved : 48;
};

/**
 * ID: 0x03
 * @param erpm: This command enables the speed control of the motor with a target
 *              ERPM. This is a signed parameter, and the sign represents the
 *              direction of the spinning. For better operation you need to tune the
 *              PID of speed control.
 *              Equation: ERPM = Motor RPM * number of the motor pole pairs
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetSpeed {
    uint64_t erpm : 32;
    uint64_t reserved : 32;
};

/**
 * ID: 0x04
 * @param position: This value targets the desired position of the motor in degrees. This
 *                  command is used to hold a position of the motor.
 *                  This feature is enabled only if encoder is used as position sensor.
 *                  The value has to be multiplied by 10 before sending.
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetPosition {
    uint64_t position : 16;
    uint64_t reserved : 48;
};


/**
 * ID: 0x05
 * @param relative_ac_current: This command sets a relative AC current to the minimum and
 *                             maximum limits set by configuration. This achieves the same
 *                             function as the “Set AC current” command. Gives you a freedom to
 *                             send values between -100,0% and 100,0%. You do not need to
 *                             know the motor limit parameters.
 *                             This value must be between -100 and 100 and must be multiplied
 *                             by 10 before sending.
 * @param unused: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetRelativeACCurrent {
    uint64_t relative_ac_current : 16;
    uint64_t reserved : 48;
};

/**
 * ID: 0x06
 * @param relative_brake_current: Targets the relative brake current of the motor. It will result negative
 *                                torque relatively to the forward direction of the motor. This value
 *                                must be between 0 and 100 and must be multiplied by 10 before
 *                                sending Gives you a freedom to send values between 0% and
 *                                100,0%. You do not need to know the motor limit parameters.
 *                                This value must be between 0 and 100 and has to be multiplied by
 *                                10 before sending
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetRelativeBrakeCurrent {
    uint64_t relative_brake_current : 16;
    uint64_t reserved : 48;
};

/**
 * ID: 0x07
 * @param digital_output_1: Sets the digital output 1 to HIGH (1) or LOW (0) state, Physical pin 2
 * @param digital_output_2: Sets the digital output 2 to HIGH (1) or LOW (0) state, Physical pin 10
 * @param digital_output_3: Sets the digital output 3 to HIGH (1) or LOW (0) state, Physical pin 19
 * @param digital_output_4: Sets the digital output 4 to HIGH (1) or LOW (0) state, Physical pin 20
 * @param reserved: Not documented, likely treated like other unused fields. Fill with FFs.
 **/
struct SetDigitalOutput {
    uint64_t digital_output_1 : 8;
    uint64_t digital_output_2 : 8;
    uint64_t digital_output_3 : 8;
    uint64_t digital_output_4 : 8;
    uint64_t reserved : 32;
}; 

/**
 * ID: 0x08
 * @param max_ac_current: This value determines the maximum allowable drive current on the
 *                        AC side. With this function you are able maximize the maximum
 *                        torque on the motor.
 *                        The value must be multiplied by 10 before sending.
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetMaxACCurrent {
    uint64_t max_ac_current : 16;
    uint64_t reserved : 48;
};

/**
 * ID: 0x09
 * @param max_brake_current: This value sets the maximum allowable brake current on the AC
 *                           side.
 *                           This value must be multiplied by 10 before sending, only negative
 *                           currents are accepted.
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetMaxBrakeCurrent {
    uint64_t max_brake_current : 16;
    uint64_t reserved : 48;
};

/**
 * ID: 0x0A
 * @param max_dc_current: This value determines the maximum allowable drive current on the
 *                        DC side. With this command the BMS can limit the maximum
 *                        allowable battery discharge current.
 *                        The value has to be multiplied by 10 before sending.
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetMaxDCCurrent {
    uint64_t max_dc_current : 16;
    uint64_t reserved : 48;
};


/**
 * ID: 0x0B
 * @param max_brake_dc_current: This value determines the maximum allowable brake current on the
 *                              DC side. With this command the BMS can limit the maximum
 *                              allowable battery charge current.
 *                              The value has to be multiplied by 10 before sending. Only negative
 *                              currents are accepted.
 * @param reserved: Not relevant to the command. Fill with FFs or use 2-byte DLC. 
 **/
struct SetMaxBrakeDCCurrent {
    uint64_t max_brake_dc_current : 16;
    uint64_t reserved : 48;
};

/**
 * ID: 0x0C
 * @param drive_enable: 0: Drive not allowed
 *                      1: Drive allowed
 *                      Only 0 and 1 values are accepted. Must be sent periodically to be
 *                      enabled. (250ms recommended)
 * @param reserved: Not relevant to the command. Fill with FFs or use 1-byte DLC. 
 **/
struct SetDriveEnable {
    uint64_t drive_enable : 8;
    uint64_t reserved : 56;
};

}; // Command
}; // Inverter