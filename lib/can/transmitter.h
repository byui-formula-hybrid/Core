#ifndef CAN_TRANSMITTER_H
#define CAN_TRANSMITTER_H

#include <core/queue.h>
#include <core/logger.h>

#include "service.h"
#include "types.h"

#ifdef IS_NATIVE
#include <atomic>
#endif

namespace CAN {

/**
* @brief A class for transmitting CAN frames.
*/
class Transmitter {
public:
    /**
     * @brief Returns the singleton instance of the transmitter.
     * @return instance: the singleton instance of the transmitter.
     */
    static Transmitter* get_instance();

    /** 
     * @brief Deleted copy constructor 
     */
    Transmitter(const Transmitter&) = delete;
    
    /** 
     * @brief Deleted assignment operator 
     */
    Transmitter& operator=(const Transmitter&) = delete;

    /** 
     * @brief Non-blocking. Adds Can Frame to the transmit queue.
     * @param frame: The frame to send.
     * @return success: True if the frame was sent successfully, false otherwise.
     */
    bool send(const Frame& frame);

    /** 
     * @brief Sets the service for transmitting frames.
     * @param service: The service to use for transmission.
     */
    void set_service(Service* service);

    /** 
     * @brief Sets the queue for transmitting frames.
     * @param queue: The queue to use for transmission.
     */
    void set_queue(Core::IQueue<Frame>* queue);

    /** 
     * @brief Transmits frames from the queue using the service. This function should be called in a loop or a dedicated task to continuously transmit frames.
     * @param data: Intended use is to pass the singleton instance into the function and to match the task callback signature
     */
    static void transmit(void* data);

#ifdef IS_NATIVE
    /**
     * @brief purely for testing purposes on native machines
     */
    std::atomic<bool> should_kill_thread{false};
#endif

private:
    /** 
     * @brief Constructor for the transmitter.
     */
    Transmitter() {}

    /** 
     * @brief The queue for transmitting frames.
     */
    Core::IQueue<Frame>* queue_tx;
    
    /** 
     * @brief The service for transmitting frames.
     */
    Service* service;
};

}

#endif // CAN_TRANSMITTER_H