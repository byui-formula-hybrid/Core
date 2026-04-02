#ifndef CORE_I_QUEUE_H
#define CORE_I_QUEUE_H

#include <cstddef>
#include <cstdint>

namespace Core {

template<typename T>
class IQueue {
public:
    virtual ~IQueue() = default;

    /**
     * @brief Adds an item to the queue.
     * @param data The item to copy into the queue.
     * @return true if successful, false if the queue is full.
     */
    virtual void enqueue(const T& data) = 0;
    
    /**
     * @brief Removes an item from the queue.
     * @param outData Reference to store the popped item.
     * @param timeout_ms How long to wait if the queue is empty (0 for non-blocking).
     * @return true if an item was retrieved, false if it timed out.
     */
    virtual bool dequeue(T& data, uint32_t timeout_ms = 0) = 0;
    virtual size_t size() const = 0;
    virtual bool is_full() const = 0;
};

} // namespace Core

#endif // CORE_I_QUEUE_H