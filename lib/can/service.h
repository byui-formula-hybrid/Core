
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
    
    /**
     * @brief Sends a CAN frame.
     * @param frame: The frame to send.
     * @return success: True if the frame was sent successfully, false otherwise.
     */
    virtual bool send(const Frame& frame) = 0;
    
    /**
     * @brief Reads a CAN frame.
     * @return pointer to the read frame, or nullptr if no frame is available.
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
    ~Service() = default;
private:
    /**
    * @brief Dispatcher for routing received frames to handlers. This is a reference to the singleton instance of the dispatcher, allowing the service to register handlers or dispatch received frames as needed.
    */
    Dispatcher& dispatcher = Dispatcher::get_instance();
};


} // namespace CAN

#endif // CAN_SERVICE_H