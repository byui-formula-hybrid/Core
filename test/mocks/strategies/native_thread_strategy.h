#pragma once

#include <thread>
#include <chrono>

#include "../../../lib/core/thread/i_thread_strategy.h"

class nativeThreadStrategy : public Core::iThreadStrategy {
private:
    std::thread m_thread;
    const char* m_name;
public:
    void setup(const char* name, uint32_t priority, uint32_t attributes) override {
        m_name = name;
        return;
    }
    uint32_t create(taskFunc task, void* argument) override {
        m_thread = std::thread(task, argument);
        return 0;
    }

    void join() override {
        m_thread.join();
    }

    void sleep(uint32_t millis) override {
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    }
};