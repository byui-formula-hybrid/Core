#ifndef CAN_TRANSMITTER_H
#define CAN_TRANSMITTER_H

#include <queue.h>
#include <logger.h>

#include "service.h"
#include "types.h"

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
    static Transmitter& get_instance();

    /** 
     * @brief Deleted copy constructor 
     */
    Transmitter(const Transmitter&) = delete;
    
    /** 
     * @brief Deleted assignment operator 
     */
    Transmitter& operator=(const Transmitter&) = delete;

    /** 
     * @brief Sends a CAN frame.
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
     */
    void transmit();

private:
    /** 
     * @brief Constructor for the transmitter.
     */
    Transmitter();

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