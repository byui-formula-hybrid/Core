#include <cstdint>

namespace Inverter {

enum class ControlMode : uint64_t {
    // UNUSED MODES 0, 5, 6
    CONTROL_MODE_SPEED = 1,
    CONTROL_MODE_CURRENT = 2,
    CONTROL_MODE_CURRENT_BRAKE = 3,
    CONTROL_MODE_POSITION = 4,
    CONTROL_MODE_NONE = 7
};

/**
 * ID: 0x1F
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

};