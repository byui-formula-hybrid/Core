#pragma once

#include <memory>

#include "commands.h"
#include "messages.h"
#include "../core/lock/i_lock_strategy.h"
#include "../core/thread/i_thread_strategy.h"
#include "../can/provider.h"
#include "../can/types.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

class Heartbeat {
private:
    bool m_started;
    bool m_shouldStop;
    std::unique_ptr<Core::iLockStrategy> m_shouldStop_mut;
    std::unique_ptr<Core::iThreadStrategy> m_thread;
    std::shared_ptr<Provider> m_canProvider;

    Command::SetDriveEnable enable;
    Command::SetDriveEnable disable;
public:
    Heartbeat(std::shared_ptr<Provider> canProvider, std::unique_ptr<Core::iLockStrategy> lock_strategy, std::unique_ptr<Core::iThreadStrategy> thread_strategy);

    void start();
    void stop();

    bool started() { return m_started; }
private:
    static void heartbeat(void* s);
};

}
}