#pragma once

#include "../../../lib/core/strategies/LockStrategy.h"
#include <mutex>
#include <memory>

namespace Core {

/**
 * @brief Native lock strategy using std::mutex for testing on host platform
 * 
 * This provides real thread safety using native C++ threading primitives,
 * allowing us to test thread-safe behavior without ESP32 hardware.
 */
class NativeLockStrategy : public LockStrategy {
private:
    std::unique_ptr<std::mutex> mutex_;

public:
    NativeLockStrategy() : mutex_(std::unique_ptr<std::mutex>(new std::mutex())) {}
    
    ~NativeLockStrategy() override = default;
    
    void lock() override {
        if (mutex_) {
            mutex_->lock();
        }
    }
    
    void unlock() override {
        if (mutex_) {
            mutex_->unlock();
        }
    }
    
    // Disable copy and move for safety
    NativeLockStrategy(const NativeLockStrategy&) = delete;
    NativeLockStrategy& operator=(const NativeLockStrategy&) = delete;
    NativeLockStrategy(NativeLockStrategy&&) = delete;
    NativeLockStrategy& operator=(NativeLockStrategy&&) = delete;
};

} // namespace Core