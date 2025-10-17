#pragma once

#include "publisher.h"
#include <memory>

namespace Core {

/**
 * @brief Generic publisher factory interface
 * 
 * This is a pure abstract interface that consuming applications must implement
 * to provide Publishers of type T. The implementation decides all details including
 * lock strategies, initialization, and platform-specific behavior.
 * 
 * Template parameter T: The type of data the publisher will broadcast
 * 
 * Example implementation:
 * ```cpp
 * class ESP32PublisherFactory : public iPublisherFactory<CANMessage> {
 * public:
 *     Publisher<CANMessage> create() override {
 *         auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
 *         return Publisher<CANMessage>(std::move(lockStrategy));
 *     }
 * };
 * ```
 * 
 * Usage:
 * ```cpp
 * ESP32PublisherFactory canFactory;
 * auto canPublisher = canFactory.create();
 * 
 * auto subscription = canPublisher.sink([](const CANMessage& msg) {
 *     // Handle CAN message
 * });
 * ```
 */
template<typename T>
class iPublisherFactory {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~iPublisherFactory() = default;
    
    /**
     * @brief Create a Publisher of type T
     * 
     * The implementation of this method is entirely up to the consuming application.
     * This includes decisions about:
     * - Thread safety (lock strategies)
     * - Memory management
     * - Platform-specific optimizations
     * - Error handling
     * 
     * @return A Publisher<T> instance configured according to implementation needs
     */
    virtual Publisher<T> create() = 0;

protected:
    /**
     * @brief Protected constructor to prevent direct instantiation
     * Only derived classes can be instantiated
     */
    iPublisherFactory() = default;
};

} // namespace Core