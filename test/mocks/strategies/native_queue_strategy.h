#ifndef NATIVE_QUEUE_STRATEGY_H
#define NATIVE_QUEUE_STRATEGY_H

#include "core/core_error.h"
#include <queue>

#include <core.h>

namespace MOCKS {
template<typename T>
class NativeQueueStrategy : public Core::IQueue<T> {
public:
    Core::QueueError enqueue(const T& data) override {
        m_queue.push(data);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return Core::QueueError::SUCCESS;
    }

    Core::QueueError enqueueFromISR(const T& data) override {
        return enqueue(data);
    }

    Core::QueueError dequeue(T& outData, uint32_t timeout_ms) override
    {
        if(m_queue.empty()) {
            return Core::QueueError::EMPTY;
        }

        outData = m_queue.front();
        m_queue.pop();
        return Core::QueueError::SUCCESS;
    }

    size_t size() const override {
        return 255; // Just to define the function
    }

    Core::QueueError is_full() override {
        return Core::QueueError::SUCCESS;
    }

private:
    std::queue<T> m_queue;
};

} // namespace MOCKS

#endif
