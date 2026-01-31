#include "controller.h"

#include "../core/lock/cmsis_os2_lock_strategy.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

Controller::Controller(std::shared_ptr<Service> canService, std::unique_ptr<Core::iLockStrategy> lock_strategy, std::unique_ptr<Core::iThreadStrategy> thread_strategy) {
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

void Controller::start() {
    // Send drive enable
    Frame frame(0x0C52, &enable);

    m_canService->transmit(&frame, 1000);

    // Start the heartbeat for drive enable
    startHeartbeat();

    m_started = true;
}

void Controller::stop() {
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

// This is the current way that the node will handle messages, still much to do
// Likely we'll want to write an error enum so that we can track the specific errors
bool Controller::handleFrame(Frame &frame) {
    switch(frame.identifier) {
        case 0x2252:
            if(getFaultCode(frame) != FaultCodes::NONE)
                return false;
            break;
        default:
            return true;
    }

    return true;
}

void Controller::startHeartbeat() {
    m_thread->create(Controller::heartbeat, this);
}

// Sends a drive enable every ~250 milliseconds so the car doesn't stop
void Controller::heartbeat(void* s) {
    Controller* self = (Controller*)s;
    for(;;) {
        self->m_thread->sleep(250U);

        // Check if it's time to stop
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

// Just interprets message22 and sends the error codes
FaultCodes Controller::getFaultCode(Frame &frame) {
    auto message = frame.decode<Message22>();

    return message->fault_code;
}

}
}