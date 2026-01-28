#pragma once

#include "../can.h"
#include "commands.h"
#include "messages.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

class CAN : public Inverter::CAN {
private:
    osThreadAttr_t m_heartbeatAttr;
    osThreadId_t m_heartbeatID;
public:
    CAN(Service* canService);

    void start();
    void stop();
    bool handleFrame(Frame& frame);
private:
    FaultCodes getFaultCode(Frame &frame);
    void startHeartbeat();

    static void heartbeat(void* s);
};

}
}