#include "heartbeat.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

Heartbeat::Heartbeat(Transmitter* canTransmitter, std::unique_ptr<Core::iLockStrategy> lock_strategy) {
    m_canTransmitter = canTransmitter;
    m_shouldStop_mut = std::move(lock_strategy);

    m_taskController = Core::TaskController::get_instance();
    m_taskController->setup_task("inverter.DTIX50.heartbeat", // name
                    0x17U, // priority - osPriorityBelowNormal7
                    0x01U  // attributes - osThreadJoinable
                   );

    m_shouldStop = false;
    m_started = false;

    enable = { 0x01, 0xFFFFFFFFFFFFFF };
    disable = { 0x00, 0xFFFFFFFFFFFFFF };
}

void Heartbeat::start(Core::iThreadStrategy* thread_strategy) {
    if(m_started) return;

    m_thread_strategy = thread_strategy;

    // Start the heartbeat for drive enable
    uint32_t handle = m_taskController->create_task(thread_strategy, Heartbeat::heartbeat, this);

    m_thread_strategy->SetHandle(handle);

    m_started = true;
}

void Heartbeat::stop() {
    // Set shouldStop so that the heartbeat knows that we're stopping
    m_shouldStop_mut->lock();
    m_shouldStop = true;
    m_shouldStop_mut->unlock();

    // Wait for the heartbeat to actually stop before sending drive disable
    m_taskController->delete_task(m_thread_strategy->GetHandle());

    m_started = false;
}

// Sends a drive enable every ~250 milliseconds so the car doesn't stop
void Heartbeat::heartbeat(void* s) {
    Heartbeat* self = (Heartbeat*)s;

    while(true) {
        
        // Check if it's time to stop
        self->m_shouldStop_mut->lock();
        if(self->m_shouldStop) 
        {
            self->m_shouldStop_mut->unlock();
            // Send drive disable
            Frame frame(0x0C52, &self->disable);
            self->m_canTransmitter->send(frame);
            return;
        }
        self->m_shouldStop_mut->unlock();
        
        // Send drive enable
        Frame frame(0x0C52, &self->enable);
        
        self->m_canTransmitter->send(frame);
        
        self->m_thread_strategy->sleep(250U);
    }
}

}
}