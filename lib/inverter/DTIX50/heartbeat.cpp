#include "heartbeat.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

Heartbeat::Heartbeat(std::shared_ptr<Service> canService, std::unique_ptr<Core::iLockStrategy> lock_strategy, std::unique_ptr<Core::iThreadStrategy> thread_strategy) {
    m_canService = canService;
    m_shouldStop_mut = std::move(lock_strategy);
    m_thread = std::move(thread_strategy);

    m_shouldStop = false;

    enable = { 0x01, 0xFFFFFFFFFFFFFF };
    disable = { 0x00, 0xFFFFFFFFFFFFFF };

    m_thread->setup("inverter.DTIX50.heartbeat", // name
                    0x17U, // priority - osPriorityBelowNormal7
                    0x01U  // attributes - osThreadJoinable
                   );
}

void Heartbeat::start() {
    if(m_started) return;

    // Send drive enable
    Frame frame(0x0C52, &enable);

    m_canService->transmit(&frame, 1000);

    // Start the heartbeat for drive enable
    m_thread->create(Heartbeat::heartbeat, this);

    m_started = true;
}

void Heartbeat::stop() {
    // Set shouldStop so that the heartbeat knows that we're stopping
    m_shouldStop_mut->lock();
    m_shouldStop = true;
    m_shouldStop_mut->unlock();

    // Send drive disable
    Frame frame(0x0C52, &disable);

    // Wait for the heartbeat to actually stop before sending drive disable
    m_thread->join();

    m_canService->transmit(&frame, 1000);

    m_started = false;
}

// Sends a drive enable every ~250 milliseconds so the car doesn't stop
void Heartbeat::heartbeat(void* s) {
    Heartbeat* self = (Heartbeat*)s;
    for(;;) {
        self->m_thread->sleep(250U);

        // Check if it's time to stop
        // TODO: Write a timeout
        self->m_shouldStop_mut->lock();
        if(self->m_shouldStop) 
        {
            self->m_shouldStop_mut->unlock();
            return;
        }
        self->m_shouldStop_mut->unlock();

        // Send drive enable
        Frame frame(0x0C52, &self->enable);

        self->m_canService->transmit(&frame, 1000);
    }
}

}
}