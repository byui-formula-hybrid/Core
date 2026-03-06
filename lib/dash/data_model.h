#pragma once

#include <cstdint>

namespace Dash {

/**
 * @brief Data model holding the latest processed vehicle state
 * 
 * This struct stores all dashboard-relevant data in real units (not raw CAN values).
 * Updated by the DashController from incoming CAN messages.
 * Read by the UI layer for rendering.
 */
struct DataModel {
    // Battery pack data
    float pack_voltage_V = 0.0f;        // [V]
    float pack_current_A = 0.0f;        // [A], positive = discharging
    float max_cell_temp_C = 0.0f;       // [°C]
    
    // Motor data
    uint32_t motor_rpm = 0;             // [RPM]
    float speed_kph = 0.0f;             // [km/h], computed from RPM + gear ratio
    
    // Pedal data
    uint8_t throttle_pct = 0;           // [0-100]
    bool brake_pressed = false;
    uint16_t pedal_adc1 = 0;            // Raw ADC value
    uint16_t pedal_adc2 = 0;            // Raw ADC value
    uint8_t pedal_error_code = 0;       // 0 = no error, [1-15] = error codes
    bool pedal_sync_fault = false;
    
    // System status flags
    bool ts_active = false;             // Tractive System active
    bool glv_on = false;                // GLV battery online
    bool ready_to_drive = false;        // RTD buzzer active
    
    // Fault flags (latched)
    bool ams_over_voltage = false;
    bool ams_under_voltage = false;
    bool ams_over_temp = false;
    bool ams_under_temp = false;
    bool ams_cell_imbalance = false;
    bool ams_crc_error = false;
    bool ams_hardware_error = false;
    bool imd_fault = false;             // Insulation monitoring fault
    
    // CAN communication status
    bool can_comm_lost = false;         // Set if no heartbeat received within timeout
    uint32_t last_heartbeat_ms = 0;     // Timestamp of last heartbeat
    
    // Pedal fault persistence tracking
    uint32_t pedal_error_cleared_ms = 0;  // Timestamp when pedal error returned to 0
    
    /**
     * Reset latched AMS fault flags
     * Called by user action (e.g., touch button on Faults page)
     */
    void reset_ams_faults() {
        ams_over_voltage = false;
        ams_under_voltage = false;
        ams_over_temp = false;
        ams_under_temp = false;
        ams_cell_imbalance = false;
        ams_crc_error = false;
        ams_hardware_error = false;
    }
    
    /**
     * Reset latched IMD fault flag
     * Called by user action (e.g., touch button on Faults page)
     */
    void reset_imd_fault() {
        imd_fault = false;
    }
    
    /**
     * Check if any AMS fault is active
     */
    bool has_ams_fault() const {
        return ams_over_voltage || ams_under_voltage || ams_over_temp || 
               ams_under_temp || ams_cell_imbalance || ams_crc_error || 
               ams_hardware_error;
    }
};

} // namespace Dash
