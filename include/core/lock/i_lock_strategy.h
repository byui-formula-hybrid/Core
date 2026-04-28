
#ifndef CORE_LOCK_I_LOCK_STRATEGY_H
#define CORE_LOCK_I_LOCK_STRATEGY_H

namespace Core {

/**
 * @brief Abstract lock strategy interface
 */
class iLockStrategy {
public:
    virtual ~iLockStrategy() = default;
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

} // namespace Core

#endif // CORE_LOCK_I_LOCK_STRATEGY_H