#pragma once

#include "../reactive/Publisher.h"
#include <memory>

namespace Core {

/**
 * @brief Factory for creating Publishers with custom lock strategies
 * 
 * Consuming software should provide their own lock strategy implementations
 * and create publishers with the appropriate strategy for their platform.
 */
class PublisherFactory {
public:
    /**
     * @brief Create publisher with custom lock strategy
     */
    template<typename T>
    static Publisher<T> create(std::unique_ptr<LockStrategy> strategy) {
        return Publisher<T>(std::move(strategy));
    }
    
    /**
     * @brief Create publisher without lock strategy (no thread safety)
     * Use only for single-threaded environments or when external synchronization is provided
     */
    template<typename T>
    static Publisher<T> createUnsafe() {
        return Publisher<T>(std::unique_ptr<LockStrategy>());
    }
};

} // namespace Core