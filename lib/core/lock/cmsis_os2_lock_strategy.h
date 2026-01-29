#pragma once

// Ensures the following code is only compiled when on an arduino
#ifndef ARDUINO_ARCH_NATIVE

#include "i_lock_strategy.h"
#include <cmsis_os2.h>
#include <portable/IAR/ARM_CM7/r0p1/portmacro.h>

namespace Core {

/**
 * @brief cmsis mutex-based lock strategy for real STM32 hardware
 * This entire file is only compiled for STM32, not for testing
 */
class CMSISOS2LockStrategy : public iLockStrategy {
private:
    osSemaphoreId_t mutex_;

public:
    CMSISOS2LockStrategy() {
        mutex_ = osSemaphoreNew();
    }

    ~CMSISOS2LockStrategy() override {
        if (mutex_) {
            osSemaphoreDelete(mutex_);
        }
    }

    void lock() override {
        if (mutex_) {
            osSemaphoreAcquire(mutex_, portMAX_DELAY)
        }
    }

    void unlock() override {
        if(mutex_) {
            osSemaphoreRelease(mutex_);
        }
    }

    // Disable copy and move
    CMSISOS2LockStrategy(const CMSISOS2LockStrategy&) = delete;
    CMSISOS2LockStrategy& operator=(const CMSISOS2LockStrategy&) = delete;
    CMSISOS2LockStrategy(CMSISOS2LockStrategy&&) = delete;
    CMSISOS2LockStrategy& operator=(CMSISOS2LockStrategy&&) = delete;
};

} // namespace Core

#endif // ARDUINO_ARCH_NATIVE