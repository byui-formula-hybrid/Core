#pragma once

#include "../reactive/CurrentValueSubject.h"
#include <memory>

namespace Core {

/**
 * @brief Factory for creating CurrentValueSubjects with custom lock strategies
 * 
 * Consuming software should provide their own lock strategy implementations
 * and create current value subjects with the appropriate strategy for their platform.
 */
class CurrentValueSubjectFactory {
public:
    /**
     * @brief Create CurrentValueSubject with custom lock strategy
     */
    template<typename T>
    static CurrentValueSubject<T> create(std::unique_ptr<LockStrategy> strategy) {
        return CurrentValueSubject<T>(std::move(strategy));
    }
    
    /**
     * @brief Create CurrentValueSubject with initial value and custom lock strategy
     */
    template<typename T>
    static CurrentValueSubject<T> create(const T& initial_value, std::unique_ptr<LockStrategy> strategy) {
        return CurrentValueSubject<T>(initial_value, std::move(strategy));
    }
    
    /**
     * @brief Create CurrentValueSubject without lock strategy (no thread safety)
     * Use only for single-threaded environments or when external synchronization is provided
     */
    template<typename T>
    static CurrentValueSubject<T> createUnsafe() {
        return CurrentValueSubject<T>(std::unique_ptr<LockStrategy>());
    }
    
    /**
     * @brief Create CurrentValueSubject with initial value but without lock strategy
     */
    template<typename T>
    static CurrentValueSubject<T> createUnsafe(const T& initial_value) {
        return CurrentValueSubject<T>(initial_value, std::unique_ptr<LockStrategy>());
    }
};

} // namespace Core