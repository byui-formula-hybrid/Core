#pragma once

#include "publisher.h"

namespace Core {

/**
 * @brief CurrentValueSubject - like Publisher but holds current value
 * 
 * Usage:
 *   CurrentValueSubject<int> temperature(25);
 *   auto subscription = temperature.sink([](int temp) {
 *       Serial.print("Temperature: ");
 *       Serial.println(temp);
 *   });
 *   temperature.send(30); // Will trigger callback immediately
 */
template<typename T>
class CurrentValueSubject : public Publisher<T> {
private:
    T current_value_;
    bool has_value_;

public:
    CurrentValueSubject() : has_value_(false) {}
    
    explicit CurrentValueSubject(const T& initial_value) 
        : current_value_(initial_value), has_value_(true) {}
    
    /**
     * @brief Constructor with dependency injection of lock strategy
     */
    explicit CurrentValueSubject(std::unique_ptr<iLockStrategy> lock_strategy) 
        : Publisher<T>(std::move(lock_strategy)), has_value_(false) {}
    
    /**
     * @brief Constructor with initial value and dependency injection
     */
    CurrentValueSubject(const T& initial_value, std::unique_ptr<iLockStrategy> lock_strategy) 
        : Publisher<T>(std::move(lock_strategy)), current_value_(initial_value), has_value_(true) {}
    
    /**
     * @brief Subscribe and immediately receive current value if available
     */
    typename Publisher<T>::Subscription sink(typename Publisher<T>::Callback callback) {
        if (has_value_) {
            callback(current_value_);
        }
        return Publisher<T>::sink(std::move(callback));
    }
    
    /**
     * @brief Send new value and store as current
     */
    void send(const T& value) {
        current_value_ = value;
        has_value_ = true;
        Publisher<T>::send(value);
    }
    
    /**
     * @brief Get current value
     */
    const T& value() const {
        return current_value_;
    }
    
    /**
     * @brief Check if has current value
     */
    bool hasValue() const {
        return has_value_;
    }
};

} // namespace Core