#pragma once

#include "../../lib/core/factories/PublisherFactory.h"
#include "../../lib/core/factories/CurrentValueSubjectFactory.h"
#include "../mocks/strategies/NativeLockStrategy.h"
#include <memory>

namespace Core {

/**
 * @brief Test utilities for creating publishers with native lock strategies
 * 
 * Uses NativeLockStrategy with std::mutex to provide real thread safety
 * during testing on native platforms.
 */
class TestPublisherFactory {
public:
    template<typename T>
    static Publisher<T> create() {
        return PublisherFactory::create<T>(
            std::unique_ptr<LockStrategy>(new NativeLockStrategy())
        );
    }
    
    template<typename T>
    static CurrentValueSubject<T> createCurrentValueSubject() {
        return CurrentValueSubjectFactory::create<T>(
            std::unique_ptr<LockStrategy>(new NativeLockStrategy())
        );
    }
    
    template<typename T>
    static CurrentValueSubject<T> createCurrentValueSubject(const T& initial_value) {
        return CurrentValueSubjectFactory::create<T>(
            initial_value,
            std::unique_ptr<LockStrategy>(new NativeLockStrategy())
        );
    }
};

} // namespace Core