#pragma once

#include <thread>
#include <chrono>

#include "../../../lib/core/thread/i_thread_strategy.h"

class NativeThreadStrategy : public Core::iThreadStrategy {
private:
    std::thread m_thread;
    const char* m_name;
public:
    void setup(const char* name, const uint32_t priority, const uint32_t attributes) override {}
    uint32_t create(taskFunc task, void* argument) override {
        m_thread = std::thread(task, argument);
        return 0;
    }

    void join() override {
        m_thread.join();
    }

    void sleep(const uint32_t millis) override {
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
    }
};