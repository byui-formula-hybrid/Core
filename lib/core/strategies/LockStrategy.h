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
 */
class LockGuard {
private:
    LockStrategy* strategy_;
    
public:
    explicit LockGuard(LockStrategy* strategy) : strategy_(strategy) {
        if (strategy_) {
            strategy_->lock();
        }
    }
    
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