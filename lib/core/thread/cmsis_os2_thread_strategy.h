#pragma once

// Ensures the following code is only compiled when on an arduino
#ifndef ARDUINO_ARCH_NATIVE

#include <cmsis_os2.h>
#include "i_thread_strategy.h"

namespace Core {

class CMSISOS2ThreadStrategy : public iThreadStrategy {
private:
    osThreadAttr_t m_attribute;
    osThreadId_t m_id;
public:
    void setup(const char* name, const uint32_t priority = osPriorityNormal, const uint32_t attributes = NULL) override {
        m_attribute = 
        {
            .name = name,
            .attr_bits = attributes,
            .priority = priority,
        }
    }
    uint32_t create(taskFunc task, void* argument) override {
       m_id = osThreadNew(task, this, &m_attribute);
       return m_id;
    }
    void join() override {
        osThreadJoin(m_id);
    }

    void sleep(const uint32_t millis) override {
        osDelay(millis);
    }
};

}

#endif