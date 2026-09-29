#ifndef CORE_I_QUEUE_H
#define CORE_I_QUEUE_H

#include <cstddef>
#include <cstdint>

#include "../core_error.h"

namespace Core {

template<typename T>
class IQueue {
public:
    /**
     * @brief Destructor for the queue.
     */
    virtual ~IQueue() = default;

    /**
     * @brief Adds an item to the queue.
     * @param data The item to copy into the queue.
     * @return QueueError
     */
    virtual QueueError enqueue(const T& data) = 0;

    /**
     * @brief Adds an item to the queue from interrupts
     * @param data The item to copy into the queue.
     * @return QueueError
     */
    virtual QueueError enqueueFromISR(const T& data) = 0;

    /**
     * @brief Removes an item from the queue.
     * @param outData Reference to store the popped item.
     * @param timeout_ms How long to wait if the queue is empty (0 for non-blocking).
     * @return QueueError
     */
    virtual QueueError dequeue(T& outData, uint32_t timeout_ms = 0) = 0;

    /**
     * @brief Returns the number of items in the queue.
     * @return size: the number of items in the queue.
     */
    virtual size_t size() const = 0;

    /**
     * @brief Checks if the queue is full.
     * @return QueueError
     */
    virtual QueueError is_full() = 0;
};

} // namespace Core

#endif // CORE_I_QUEUE_H
