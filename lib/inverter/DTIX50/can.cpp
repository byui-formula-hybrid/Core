#include "can.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

CAN::CAN(Service *canService) {
    m_canService = canService;

    enable = { 0x01, 0xFFFFFFFFFFFFFF };
    disable = { 0x00, 0xFFFFFFFFFFFFFF };

    m_heartbeatAttr = 
    {   
        .name = "DTIX50.heartbeat", // Thread name for debugging
        .attr_bits = osThreadJoinable, // We wait for the thread to join before sending a drive disable
        .priority = osPriorityAboveNormal // Make sure that this thread has some priority
    };
}

void CAN::start() {
    // Send drive enable
    Frame frame(0x0C52, &enable);

    m_canService->transmit(&frame, 1000);

    // Start the heartbeat for drive enable
    startHeartbeat();
}

void CAN::stop() {
    // Set shouldStop so that the heartbeat knows that we're stopping
    m_shouldStop_mut.lock();
    m_shouldStop = true;
    m_shouldStop_mut.unlock();

    // Send drive disable
    Frame frame(0x0C52, &disable);

    // Wait for the heartbeat to actually stop before sending drive disable
    osThreadJoin(m_heartbeatID);

    m_canService->transmit(&frame, 1000);
}

// This is the current way that the node will handle messages, still much to do
// Likely we'll want to write an error enum so that we can track the specific errors
bool CAN::handleFrame(Frame &frame) {
    switch(frame.identifier) {
        case 0x2252:
            if(getFaultCode(frame) != FaultCodes::NONE)
                return false;
            break;
        default:
            return true;
    }
}

void CAN::startHeartbeat() {
    m_heartbeatID = osThreadNew(CAN::heartbeat, this, &m_heartbeatAttr);
}

// Sends a drive enable every ~250 milliseconds so the car doesn't stop
void CAN::heartbeat(void* s) {
    CAN* self = (CAN*)s;
    for(;;) {
        osDelay(250U);

        // Check if it's time to stop
        self->m_shouldStop_mut.lock();
        if(self->m_shouldStop) return;
        self->m_shouldStop_mut.unlock();

        // Send drive enable
        Frame frame(0x0C52, &enable);

        self->m_canService->transmit(&frame, 1000);
    }
}

// Just interprets message22 and sends the error codes
FaultCodes CAN::getFaultCode(Frame &frame) {
    auto message = frame.decode<Message22>();

    return message->fault_code;
}

}
}