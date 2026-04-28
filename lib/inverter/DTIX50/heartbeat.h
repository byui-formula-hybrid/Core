#ifndef INVERTER_DTIX50_HEARTBEAT_H
#define INVERTER_DTIX50_HEARTBEAT_H

#include <memory>

#include <core.h>
#include <can.h>
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
    std::unique_ptr<Core::iThreadStrategy> m_thread;
    Transmitter* m_canTransmitter;

    Command::SetDriveEnable enable;
    Command::SetDriveEnable disable;
public:
    Heartbeat(Transmitter* canTransmitter, std::unique_ptr<Core::iLockStrategy> lock_strategy, std::unique_ptr<Core::iThreadStrategy> thread_strategy);

    void start();
    void stop();

    bool started() { return m_started; }
private:
    static void heartbeat(void* s);
};

}

}

#endif // INVERTER_DTIX50_HEARTBEAT_H