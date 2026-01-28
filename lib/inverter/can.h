#pragma once

#include "../core/lock/cmsis_os2_lock_strategy.h"
#include "../can/provider.h"
#include "../can/types.h"

using namespace CAN;

namespace Inverter {

// Virtual base class that we can use to implement for any inverter
class CAN {
protected:
    bool m_started;
    bool m_shouldStop;
    Core::CMSISOS2LockStrategy m_shouldStop_mut;
    Service *m_canService;
public:
    virtual bool handleFrame(Frame &frame);
    virtual void start();
    virtual void stop();
};

}