#pragma once

#include <memory>

#include "../core/lock/i_lock_strategy.h"
#include "../core/thread/i_thread_strategy.h"
#include "../can/provider.h"
#include "../can/types.h"

using namespace CAN;

namespace Inverter {

/**
 * @brief Virtual base class that we can use to implement for any inverter
 * 
 * @param m_started: Should be set by start() to indicate that the controller is active
 * @param m_shouldStop: Made for use with multithreaded controllers to tell threads to stop
 * @param m_shouldStop_mut: For use in conjunction with m_shouldStop to verify safety in reading and writing
 * @param m_thread: The thread strategy for a platform-agnostic threading solution
 * @param m_canService: The Service for transmitting/recieving on the CAN Bus
 * 
 * @fn handleFrame: The function to be called to interpret specific messages for the device
 *                  Currently returns false if there is an error, or true if all is good
 * @fn start: used to send any startup commands, also used to start the m_thread, sets m_started to true
 * @fn stop:  used to send any shutdown commands, should set m_shouldStop to false to stop threads, set m_started to false
 * @fn started: returns m_started
 */

class Controller {
protected:
    bool m_started;
    bool m_shouldStop;
    std::unique_ptr<Core::iLockStrategy> m_shouldStop_mut;
    std::unique_ptr<Core::iThreadStrategy> m_thread;
    std::shared_ptr<Service> m_canService;
public:
    virtual bool handleFrame(Frame &frame) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    bool started() { return m_started; }
};

}