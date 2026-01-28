#pragma once

#include <mutex>

#include "../can/provider.h"
#include "../can/types.h"

using namespace CAN;

namespace Inverter {

// Virtual base class that we can use to implement for any inverter
class CAN {
protected:
    bool m_started;
    bool m_shouldStop;
    std::mutex m_shouldStop_mut;
    Service *m_canService;
public:
    virtual bool handleFrame(Frame &frame);
    virtual void start();
    virtual void stop();
};

}