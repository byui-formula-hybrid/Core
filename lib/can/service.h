
#ifndef CAN_SERVICE_H
#define CAN_SERVICE_H

#include <stdint.h>
#include <memory>
#include "types.h"
#include "dispatcher.h"

namespace CAN {

/**
* @brief Base class for CAN services.
*/
class Service {
public:
    /**
     * @brief Sets up the CAN service.
     * @param config: A pointer to the service configuration.
     * @return success: True if the service was set up successfully, false otherwise.
     */
    virtual bool setup(const void * config) = 0;
    
    /**
     * @brief Tears down the CAN service.
     * @return success: True if the service was torn down successfully, false otherwise.
     */
    virtual bool tear_down() = 0;

    virtual void stop_listening() = 0;

    virtual void start_listening() = 0;
    
    /**
     * @brief Sends a CAN frame.
     * @param frame: The frame to send.
     * @return success: True if the frame was sent successfully, false otherwise.
     */
    virtual bool send(const Frame& frame) = 0;
    
    /**
     * @brief Reads a CAN frame and passes it to the dispatch callback
     * @return pointer to the read frame, or nullptr if no frame is available.
     * TODO: Figure out if we actually need a return here
     */
    virtual const Frame* read() = 0;
    
    /**
     * @brief Attempts to recover the CAN service from an error state.
     * @return success: True if recovery was successful, false otherwise.
     */
    virtual bool recover() = 0;

    /**
    * @brief Destructor for the CAN service.
    */
    virtual ~Service() = default;
private:
    /**
    * @brief Callback that is called from read to pass the incoming data out
    */
    void (*dispatch)(void* data);
};


} // namespace CAN

#endif // CAN_SERVICE_H