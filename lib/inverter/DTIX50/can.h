#pragma once

#include "../can.h"
#include "commands.h"
#include "messages.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

class CAN : public Inverter::CAN {
private:
    Command::SetDriveEnable enable;
    Command::SetDriveEnable disable;
public:
    CAN(Service* canService, std::unique_ptr<iLockStrategy> lock_strategy, std::unique_ptr<iThreadStrategy> thread_strategy);

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