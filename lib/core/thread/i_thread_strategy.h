#pragma once

typedef void(*taskFunc)(void*);

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
    virtual ~iThreadStrategy() = default;
    virtual void setup(const char* name = nullptr, const uint32_t priority = 0, const uint32_t attributes = 0) = 0;
    virtual uint32_t create(taskFunc task, void* argument) = 0;
    virtual void join() = 0;
    virtual void sleep(uint32_t millis) = 0;
};

}