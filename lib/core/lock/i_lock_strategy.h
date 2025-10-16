#pragma once

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