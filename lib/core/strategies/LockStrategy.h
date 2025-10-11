#pragma once

namespace Core {

/**
 * @brief Abstract lock strategy interface
 */
class LockStrategy {
public:
    virtual ~LockStrategy() = default;
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

/**
 * @brief RAII lock guard that works with any lock strategy
 * 
 * This class acquires the lock in its constructor and releases it in its destructor.
 * This ensures that the lock is always released, even if the function exits early or throws.
 */
class LockGuard {
private:
    LockStrategy* strategy_;
    
public:
    /**
     * @brief Acquire the lock on construction
     * @param strategy Pointer to the lock strategy (can be nullptr for no locking)
     */
    explicit LockGuard(LockStrategy* strategy) : strategy_(strategy) {
        if (strategy_) {
            strategy_->lock();
        }
    }
    /**
     * @brief Release the lock on destruction
     * This is called automatically when the LockGuard goes out of scope.
     */
    ~LockGuard() {
        if (strategy_) {
            strategy_->unlock();
        }
    }
    // Disable copy and move to prevent issues
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
    LockGuard(LockGuard&&) = delete;
    LockGuard& operator=(LockGuard&&) = delete;
};

} // namespace Core