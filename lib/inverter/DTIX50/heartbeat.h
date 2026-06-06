#ifndef INVERTER_DTIX50_HEARTBEAT_H
#define INVERTER_DTIX50_HEARTBEAT_H

#include <memory>

#include <core.h>
#include <core_can.h>
#include <core/task.h>
#include "commands.h"
#include "messages.h"

using namespace CAN;

namespace Inverter {
namespace DTIX50 {

class Heartbeat {
private:
    bool m_started;
    bool m_shouldStop;
    std::unique_ptr<Core::iLockStrategy> m_shouldStop_mut;
    Core::TaskController* m_taskController;
    Core::iThreadStrategy* m_thread_strategy;
    uint32_t m_thread; // A thread handle
    Transmitter* m_canTransmitter;

    Command::SetDriveEnable enable;
    Command::SetDriveEnable disable;
public:
    Heartbeat(Transmitter* canTransmitter, std::unique_ptr<Core::iLockStrategy> lock_strategy);

    void start(Core::iThreadStrategy* thread_strategy);
    void stop();

    bool started() { return m_started; }
private:
    static void heartbeat(void* s);
};

}

}

#endif // INVERTER_DTIX50_HEARTBEAT_H