#pragma once

#include "LockStrategy.h"

// Only compile this entire file for real ESP32 hardware
#ifndef ARDUINO_ARCH_NATIVE

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace Core {

/**
 * @brief FreeRTOS mutex-based lock strategy for real ESP32 hardware
 * This entire file is only compiled for ESP32, not for testing
 */
class FreeRTOSLockStrategy : public LockStrategy {
private:
    SemaphoreHandle_t mutex_;

public:
    FreeRTOSLockStrategy() {
        mutex_ = xSemaphoreCreateMutex();
    }
    
    ~FreeRTOSLockStrategy() override {
        if (mutex_) {
            vSemaphoreDelete(mutex_);
        }
    }
    
    void lock() override {
        if (mutex_) {
            xSemaphoreTake(mutex_, portMAX_DELAY);
        }
    }
    
    void unlock() override {
        if (mutex_) {
            xSemaphoreGive(mutex_);
        }
    }
    
    // Disable copy and move
    FreeRTOSLockStrategy(const FreeRTOSLockStrategy&) = delete;
    FreeRTOSLockStrategy& operator=(const FreeRTOSLockStrategy&) = delete;
    FreeRTOSLockStrategy(FreeRTOSLockStrategy&&) = delete;
    FreeRTOSLockStrategy& operator=(FreeRTOSLockStrategy&&) = delete;
};

} // namespace Core

#endif // ARDUINO_ARCH_NATIVE