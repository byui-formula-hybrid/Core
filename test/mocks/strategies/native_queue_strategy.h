#ifndef NATIVE_QUEUE_STRATEGY_H
#define NATIVE_QUEUE_STRATEGY_H

#include <queue>

#include <core.h>

namespace MOCKS {
template<typename T>
class NativeQueueStrategy : public Core::IQueue<T> {
public:
    bool enqueue(const T& data) override {
        m_queue.push(data);

        return true;
    }

    bool dequeue(T& outData, uint32_t timeout_ms) override
    {
        if(m_queue.empty()) {
            return false;
        }

        outData = m_queue.front();
        m_queue.pop();
        return true;
    }

    size_t size() const override {
        return 255; // Just to define the function
    }

    bool is_full() override {
        return false;
    }

private:
    std::queue<T> m_queue;
};

} // namespace MOCKS

#endif