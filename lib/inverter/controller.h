#pragma once

#include <memory>

#include "../core/lock/i_lock_strategy.h"
#include "../core/thread/i_thread_strategy.h"
#include "../can/provider.h"
#include "../can/types.h"

using namespace CAN;

namespace Inverter {

// Virtual base class that we can use to implement for any inverter
class Controller {
protected:
    bool m_started;
    bool m_shouldStop;
    std::unique_ptr<Core::iLockStrategy> m_shouldStop_mut;
    std::unique_ptr<Core::iThreadStrategy> m_thread;
    Service *m_canService;
public:
    virtual bool handleFrame(Frame &frame) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    bool started() { return m_started; }
};

}