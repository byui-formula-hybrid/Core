
#ifndef CORE_THREAD_I_THREAD_STRATEGY_H
#define CORE_THREAD_I_THREAD_STRATEGY_H

typedef void(*taskFunc)(void*);

#include <cstdint>

namespace Core {

/**
 * @brief Multithreading abstract interface
 *          In the case of FreeRTOS this would also be the interface for tasks.
 * @fn setup: Used with freeRTOS and CMSIS in mind to setup attributes
 * @fn create: Actually creates the thread, thread will be running after this function
 * @fn join: Blocks until the thread dies
 * @fn sleep: called from inside of the thread to cause the thread to sleep
 */
class iThreadStrategy {
public:
    iThreadStrategy() = default;
    virtual ~iThreadStrategy() = default;
    virtual void setup(const char* name = nullptr, const uint32_t priority = 0, const uint32_t attributes = 0) = 0;
    virtual uint32_t create(taskFunc task, void* argument) = 0;
    virtual void kill() = 0;
    virtual void sleep(const uint32_t millis) = 0;
    
    /**
    * @brief Set the thread Handle
    * @param handle: the thread handle
    */
    void SetHandle(uint32_t handle) {
        m_handle = handle;
    }

    /**
    * @brief Get the thread Handle
    * @return the thread handle
    */
    uint32_t GetHandle() {
        return m_handle;
    }
protected:
    uint32_t m_handle;
};

}

#endif // CORE_THREAD_I_THREAD_STRATEGY_H