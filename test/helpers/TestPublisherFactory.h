#pragma once

#include "../../lib/core/factories/PublisherFactory.h"
#include "../../lib/core/factories/CurrentValueSubjectFactory.h"
#include "../mocks/strategies/NativeLockStrategy.h"
#include <memory>

namespace Core {

/**
 * @brief Test implementation of publisher factory using native lock strategy
 * 
 * Provides thread-safe publishers for testing using std::mutex instead of FreeRTOS.
 * This allows testing reactive behavior on the host platform.
 */
template<typename T>
class TestPublisherFactory : public iPublisherFactory<T> {
public:
    /**
     * @brief Create publisher with native lock strategy for testing
     */
    Publisher<T> create() override {
        return Publisher<T>(std::unique_ptr<LockStrategy>(new NativeLockStrategy()));
    }
};

/**
 * @brief Test implementation of CurrentValueSubject factory using native lock strategy
 */
template<typename T>
class TestCurrentValueSubjectFactory : public iCurrentValueSubjectFactory<T> {
public:
    /**
     * @brief Create subject without initial value
     */
    CurrentValueSubject<T> create() override {
        return CurrentValueSubject<T>(std::unique_ptr<LockStrategy>(new NativeLockStrategy()));
    }
    
    /**
     * @brief Create subject with initial value
     */
    CurrentValueSubject<T> create(const T& initial_value) override {
        return CurrentValueSubject<T>(initial_value, std::unique_ptr<LockStrategy>(new NativeLockStrategy()));
    }
};

/**
 * @brief Convenience functions for common test scenarios
 */
class TestFactoryHelpers {
public:
    /**
     * @brief Quick publisher creation for common test types
     */
    template<typename T>
    static Publisher<T> createPublisher() {
        TestPublisherFactory<T> factory;
        return factory.create();
    }
    
    /**
     * @brief Quick subject creation for common test types
     */
    template<typename T>
    static CurrentValueSubject<T> createSubject() {
        TestCurrentValueSubjectFactory<T> factory;
        return factory.create();
    }
    
    template<typename T>
    static CurrentValueSubject<T> createSubject(const T& initial_value) {
        TestCurrentValueSubjectFactory<T> factory;
        return factory.create(initial_value);
    }
};

} // namespace Core