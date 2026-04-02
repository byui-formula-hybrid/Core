#ifndef CAN_DISPATCHER_H
#define CAN_DISPATCHER_H

#include <map>
#include <memory>
#include <queue.h>
#include <logger.h>

#include "types.h"

class watchdog;

namespace CAN {

/**
* @brief Class for dispatching CAN frames to appropriate handlers.
*/
class Dispatcher {
public:
    /**
     * @brief Returns the singleton instance of the dispatcher.
     * @return instance: the singleton instance of the dispatcher.
     */
    static Dispatcher& get_instance();

    /**
     * @brief Deleted copy constructor to prevent copying of the singleton instance.
     */
    Dispatcher(const Dispatcher&) = delete;

    /**
    * @brief Enqueues a CAN frame for dispatching. This function is typically called by the CAN service when a new frame is received. The frame will be added to the internal queue and processed in the order it was received.
    * @param data: The CAN frame to enqueue.
    */
    Dispatcher& operator=(const Dispatcher&) = delete;

    /**
     * @brief Enqueues a CAN frame for dispatching. This function is typically called by the CAN service when a new frame is received. The frame will be added to the internal queue and processed in the order it was received.
     * @param data: The CAN frame to enqueue.
     */
    void enqueue(const Frame& data);

    /**
     * @brief Registers a route for a specific CAN ID.
     * @param id: The CAN ID to register.
     * @param handler: The handler to register for the ID.
     */
    void register_route(uint32_t id, IHandler* handler);

    /**
     * @brief Dispatches frames from the queue to the appropriate handlers based on their CAN ID. This function should be called in a loop or a dedicated task to continuously process incoming frames.
     */
    void dispatch();

    /**
     * @brief Sets the queue for receiving frames to be dispatched.
     * @param q: The queue to use for receiving frames.
     */
    void set_queue(Core::IQueue<Frame>* q) { queue_rx = q; }

    /**
     * @brief Sets the maximum number of routes that can be registered.
     * @param max: The maximum number of routes.
     */
    void set_max_routes(size_t max) {
        max_routes = max;
        delete[] routes;
        routes = new IHandler*[max_routes](); // Initialize all to nullptr
    }

    /**
     * @brief Destructor for the dispatcher.
     */
    ~Dispatcher() {
        delete[] routes;
    }
private:
    /**
    * @brief Constructor for the dispatcher. This is private to enforce the singleton pattern.
    */
    Dispatcher() {
        routes = new IHandler*[max_routes]();
    }

    /**
    * @brief The queue for receiving frames to be dispatched.
    */
    Core::IQueue<Frame>* queue_rx;

    /**
    * @brief An array of pointers to handlers for each possible CAN ID.
    */
    IHandler** routes;

    /**
    * @brief The maximum number of routes that can be registered.
    */
    size_t max_routes = 0;
};

} // namespace CAN

#endif // CAN_DISPATCHER_H