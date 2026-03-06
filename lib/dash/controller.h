#pragma once

#include <memory>
#include <cstdint>
#include "data_model.h"
#include "messages.h"
#include "../core/lock/i_lock_strategy.h"
#include "../core/thread/i_thread_strategy.h"
#include "../can/provider.h"
#include "../can/types.h"

namespace Dash {

/**
 * @brief Dashboard controller - receives CAN frames and updates DataModel
 * 
 * Runs a background thread that continuously polls the CAN bus for incoming messages.
 * Decodes messages, applies scale factors, implements fault latching logic,
 * and detects CAN timeout conditions.
 */
class Controller {
private:
    bool m_started;
    bool m_shouldStop;
    std::unique_ptr<Core::iLockStrategy> m_shouldStop_mut;
    std::unique_ptr<Core::iLockStrategy> m_data_mut;
    std::unique_ptr<Core::iThreadStrategy> m_thread;
    std::shared_ptr<CAN::Provider> m_canProvider;
    
    DataModel m_data;
    
public:
    /**
     * @brief Construct a new Dashboard Controller
     * 
     * @param canProvider Shared CAN provider for receiving messages
     * @param lock_strategy Lock strategy for thread-safe data access
     * @param thread_strategy Thread strategy for creating the CAN polling thread
     */
    Controller(
        std::shared_ptr<CAN::Provider> canProvider,
        std::unique_ptr<Core::iLockStrategy> shouldStop_lock,
        std::unique_ptr<Core::iLockStrategy> data_lock,
        std::unique_ptr<Core::iThreadStrategy> thread_strategy
    );
    
    /**
     * @brief Start the CAN receive loop thread
     */
    void start();
    
    /**
     * @brief Stop the CAN receive loop thread
     */
    void stop();
    
    /**
     * @brief Check if the controller is running
     */
    bool started() const { return m_started; }
    
    /**
     * @brief Get a thread-safe snapshot of the current data model
     * @return DataModel Copy of current state
     */
    DataModel snapshot();
    
    /**
     * @brief Reset latched AMS faults
     */
    void reset_ams_faults();
    
    /**
     * @brief Reset latched IMD fault
     */
    void reset_imd_fault();
    
private:
    /**
     * @brief CAN receive loop (runs in background thread)
     * @param s Pointer to Controller instance
     */
    static void poll(void* s);
    
    /**
     * @brief Get current time in milliseconds (platform-dependent)
     */
    uint32_t millis();
    
    /**
     * @brief Handle incoming CAN frame
     * @param frame The received CAN frame
     */
    void handleFrame(CAN::Frame& frame);
};

} // namespace Dash
