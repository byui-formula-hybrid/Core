#pragma once

#include "../reactive/CurrentValueSubject.h"
#include <memory>

namespace Core {

/**
 * @brief Generic CurrentValueSubject factory interface
 * 
 * This is a pure abstract interface that consuming applications must implement
 * to provide CurrentValueSubjects of type T. The implementation decides all details
 * including lock strategies, initialization, and platform-specific behavior.
 * 
 * Template parameter T: The type of data the subject will hold and broadcast
 * 
 * Example implementation:
 * ```cpp
 * class ESP32SubjectFactory : public iCurrentValueSubjectFactory<float> {
 * public:
 *     CurrentValueSubject<float> create() override {
 *         auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
 *         return CurrentValueSubject<float>(std::move(lockStrategy));
 *     }
 *     
 *     CurrentValueSubject<float> create(const float& initial_value) override {
 *         auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
 *         return CurrentValueSubject<float>(initial_value, std::move(lockStrategy));
 *     }
 * };
 * ```
 * 
 * Usage:
 * ```cpp
 * ESP32SubjectFactory batteryFactory;
 * auto batteryLevel = batteryFactory.create(85.5f); // Start with 85.5%
 * 
 * auto subscription = batteryLevel.sink([](float level) {
 *     // Handle battery level change
 * });
 * ```
 */
template<typename T>
class iCurrentValueSubjectFactory {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~iCurrentValueSubjectFactory() = default;
    
    /**
     * @brief Create a CurrentValueSubject of type T without initial value
     * 
     * Creates a subject that starts without a value. Subscribers will only
     * receive values after the first send() call.
     * 
     * @return A CurrentValueSubject<T> instance configured according to implementation needs
     */
    virtual CurrentValueSubject<T> create() = 0;
    
    /**
     * @brief Create a CurrentValueSubject of type T with initial value
     * 
     * Creates a subject with an initial value. New subscribers will immediately
     * receive this initial value when they subscribe.
     * 
     * @param initial_value The initial value to store in the subject
     * @return A CurrentValueSubject<T> instance with the specified initial value
     */
    virtual CurrentValueSubject<T> create(const T& initial_value) = 0;

protected:
    /**
     * @brief Protected constructor to prevent direct instantiation
     * Only derived classes can be instantiated
     */
    iCurrentValueSubjectFactory() = default;
};

} // namespace Core