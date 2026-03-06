#include "controller.h"
#include <cstring>

#ifndef ARDUINO_ARCH_NATIVE
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
#else
    #include <chrono>
#endif

namespace Dash {

Controller::Controller(
    std::shared_ptr<CAN::Provider> canProvider,
    std::unique_ptr<Core::iLockStrategy> shouldStop_lock,
    std::unique_ptr<Core::iLockStrategy> data_lock,
    std::unique_ptr<Core::iThreadStrategy> thread_strategy
) {
    m_canProvider = canProvider;
    m_shouldStop_mut = std::move(shouldStop_lock);
    m_data_mut = std::move(data_lock);
    m_thread = std::move(thread_strategy);
    
    m_shouldStop = false;
    m_started = false;
    
    // Initialize data model to safe defaults
    m_data = DataModel();
    
    m_thread->setup("dash.controller", 0x17U, 0x01U);
}

void Controller::start() {
    if (m_started) return;
    
    m_thread->create(Controller::poll, this);
    m_started = true;
}

void Controller::stop() {
    m_shouldStop_mut->lock();
    m_shouldStop = true;
    m_shouldStop_mut->unlock();
    
    m_thread->join();
    m_started = false;
}

DataModel Controller::snapshot() {
    m_data_mut->lock();
    DataModel copy = m_data;
    m_data_mut->unlock();
    return copy;
}

void Controller::reset_ams_faults() {
    m_data_mut->lock();
    m_data.reset_ams_faults();
    m_data_mut->unlock();
}

void Controller::reset_imd_fault() {
    m_data_mut->lock();
    m_data.reset_imd_fault();
    m_data_mut->unlock();
}

uint32_t Controller::millis() {
#ifndef ARDUINO_ARCH_NATIVE
    return (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
#else
    auto now = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return static_cast<uint32_t>(ms.count());
#endif
}

void Controller::handleFrame(CAN::Frame& frame) {
    uint32_t now = millis();
    
    switch (frame.identifier) {
        case CANID::AMSVoltCurrent: {
            auto* msg = frame.decode<AMSVoltCurrent>();
            m_data.pack_voltage_V = msg->pack_voltage_raw * 0.1f;
            m_data.pack_current_A = static_cast<int16_t>(msg->pack_current_raw) * 0.1f;
            break;
        }
        
        case CANID::AMSTemperature: {
            auto* msg = frame.decode<AMSTemperature>();
            m_data.max_cell_temp_C = msg->max_cell_temp_raw * 0.1f;
            break;
        }
        
        case CANID::AMSFault: {
            auto* msg = frame.decode<AMSFault>();
            // Latch faults (they don't auto-clear)
            if (msg->over_voltage) m_data.ams_over_voltage = true;
            if (msg->under_voltage) m_data.ams_under_voltage = true;
            if (msg->over_temp) m_data.ams_over_temp = true;
            if (msg->under_temp) m_data.ams_under_temp = true;
            if (msg->cell_imbalance) m_data.ams_cell_imbalance = true;
            if (msg->crc_error) m_data.ams_crc_error = true;
            if (msg->hardware_error) m_data.ams_hardware_error = true;
            break;
        }
        
        case CANID::IMDFault: {
            auto* msg = frame.decode<IMDFault>();
            // Latch fault
            if (msg->iso_fault) m_data.imd_fault = true;
            break;
        }
        
        case CANID::MotorStatus: {
            auto* msg = frame.decode<MotorStatus>();
            m_data.motor_rpm = msg->motor_rpm_raw;
            // Compute speed from RPM (placeholder formula)
            // TODO: Replace with actual gear ratio and wheel circumference
            // Example: speed_kph = (rpm * wheel_circumference_m * 60) / (gear_ratio * 1000)
            m_data.speed_kph = (m_data.motor_rpm * 0.05f);  // Placeholder
            break;
        }
        
        case CANID::VCUStatus: {
            auto* msg = frame.decode<VCUStatus>();
            m_data.ts_active = msg->ts_active;
            m_data.glv_on = msg->glv_on;
            m_data.ready_to_drive = msg->ready_to_drive;
            break;
        }
        
        case CANID::PedalData: {
            auto* msg = frame.decode<PedalData>();
            m_data.throttle_pct = msg->throttle_pct_raw;
            m_data.brake_pressed = msg->brake_pressed;
            m_data.pedal_adc1 = msg->pedal_adc1_raw;
            m_data.pedal_adc2 = msg->pedal_adc2_raw;
            
            // Pedal fault persistence: keep error active for at least 2 seconds after clearing
            if (msg->pedal_error_code > 0) {
                m_data.pedal_error_code = msg->pedal_error_code;
                m_data.pedal_error_cleared_ms = 0;  // Reset clear timer
            } else if (m_data.pedal_error_code > 0) {
                // Error code returned to 0, start persistence timer
                if (m_data.pedal_error_cleared_ms == 0) {
                    m_data.pedal_error_cleared_ms = now;
                } else if (now - m_data.pedal_error_cleared_ms >= 2000) {
                    // 2 seconds elapsed, clear the error
                    m_data.pedal_error_code = 0;
                    m_data.pedal_error_cleared_ms = 0;
                }
            }
            
            m_data.pedal_sync_fault = msg->pedal_sync_fault;
            break;
        }
        
        case CANID::Heartbeat: {
            auto* msg = frame.decode<Heartbeat>();
            m_data.last_heartbeat_ms = now;
            m_data.can_comm_lost = false;  // Heartbeat received, CAN is alive
            (void)msg;  // Suppress unused variable warning
            break;
        }
        
        default:
            // Unknown CAN ID, ignore
            break;
    }
}

void Controller::poll(void* s) {
    Controller* self = (Controller*)s;
    CAN::Frame frame;
    
    for (;;) {
        // Check if we should stop
        self->m_shouldStop_mut->lock();
        if (self->m_shouldStop) {
            self->m_shouldStop_mut->unlock();
            return;
        }
        self->m_shouldStop_mut->unlock();
        
        // Try to receive a CAN frame (50ms timeout)
        bool did_receive = self->m_canProvider->receive(frame, 50);
        
        self->m_data_mut->lock();
        
        if (did_receive) {
            // Process the frame
            self->handleFrame(frame);
        }
        
        // Check for CAN timeout (no heartbeat in last 500ms)
        uint32_t now = self->millis();
        if (self->m_data.last_heartbeat_ms > 0 && 
            (now - self->m_data.last_heartbeat_ms) > 500) {
            self->m_data.can_comm_lost = true;
        }
        
        self->m_data_mut->unlock();
    }
}

} // namespace Dash
