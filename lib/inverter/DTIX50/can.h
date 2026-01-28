#pragma once

#include <cmsis_os2.h> // We need to generate the right code for the board we end up getting

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

    void start() override;
    void stop() override;
    bool handleFrame(Frame& frame) override;
private:
    FaultCodes getFaultCode(Frame &frame);
    void startHeartbeat();

    static void heartbeat(void* s);
};

}
}