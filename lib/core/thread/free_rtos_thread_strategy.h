#pragma once

#ifndef ARDUINO_ARCH_NATIVE

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "i_thread_strategy.h"

namespace Core {

class FreeRTOSThreadStrategy : public iThreadStrategy {
public:
    FreeRTOSThreadStrategy() : m_task(nullptr), m_priority(1U), m_name("task") {
    }

    void setup(const char* name = nullptr, const uint32_t priority = 1U, const uint32_t attributes = 0U) override {
        (void)attributes;
        m_name = (name == nullptr) ? "task" : name;
        m_priority = priority;
    }

    uint32_t create(taskFunc task, void* argument) override {
        if (task == nullptr) {
            return 0U;
        }

        BaseType_t rc = xTaskCreate(
            task,
            m_name,
            4096,
            argument,
            static_cast<UBaseType_t>(m_priority),
            &m_task);

        return (rc == pdPASS) ? 1U : 0U;
    }

    void join() override {
        // FreeRTOS tasks do not have a direct join primitive in this abstraction.
        // Dashboard controller stop() sets a stop flag before this call.
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    void sleep(const uint32_t millis) override {
        vTaskDelay(pdMS_TO_TICKS(millis));
    }

private:
    TaskHandle_t m_task;
    uint32_t m_priority;
    const char* m_name;
};

}  // namespace Core

#endif  // ARDUINO_ARCH_NATIVE
