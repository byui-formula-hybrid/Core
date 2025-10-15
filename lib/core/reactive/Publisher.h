#pragma once

#include <vector>
#include <functional>
#include <memory>
#include <cstddef>
#include <algorithm>
#include "../strategies/LockStrategy.h"
#include "Subscription.h"

namespace Core {

/**
 * @brief Simple Publisher class inspired by Swift Combine
 * 
 * Usage:
 *   Publisher<int> numberPublisher;
 *   auto subscription = numberPublisher.sink([](int value) {
 *       Serial.println(value);
 *   });
 *   numberPublisher.send(42);
 */
template<typename T>
class Publisher {
public:
    using Callback = std::function<void(const T&)>;
    using Subscription = Core::Subscription<T>;

private:
    struct Subscriber {
        size_t id;
        Callback callback;
    };
    
    std::vector<Subscriber> subscribers_;
    size_t next_id_ = 1;
    std::unique_ptr<LockStrategy> lock_strategy_;

public:
    /**
     * @brief Constructor with dependency injection of lock strategy
     * @param lock_strategy The locking strategy to use (ownership transferred)
     */
    explicit Publisher(std::unique_ptr<LockStrategy> lock_strategy) 
        : lock_strategy_(std::move(lock_strategy)) {}
    
    /**
     * @brief Default constructor - creates publisher without thread safety
     * For thread-safe publishers, use constructor with lock strategy
     */
    Publisher() : lock_strategy_(nullptr) {}    ~Publisher() = default;
    
    /**
     * @brief Subscribe to this publisher with a callback
     * @param callback Function to call when values are published
     * @return Subscription handle that unsubscribes when destroyed
     */
    Subscription sink(Callback callback) {
        LockGuard guard(lock_strategy_.get());
        size_t id = next_id_++;
        subscribers_.push_back({id, std::move(callback)});
        return Subscription(this, id);
    }
    
    /**
     * @brief Publish a value to all subscribers
     * @param value The value to send
     */
    void send(const T& value) {
        LockGuard guard(lock_strategy_.get());
        for (const auto& subscriber : subscribers_) {
            if (subscriber.callback) {
                subscriber.callback(value);
            }
        }
    }
    
    /**
     * @brief Get number of active subscribers
     */
    size_t subscriberCount() const {
        return subscribers_.size();
    }

    // Disable copy
    Publisher(const Publisher&) = delete;
    Publisher& operator=(const Publisher&) = delete;
    
    // Enable move
    Publisher(Publisher&& other) noexcept 
        : subscribers_(std::move(other.subscribers_)), 
          next_id_(other.next_id_),
          lock_strategy_(std::move(other.lock_strategy_)) {
    }
    
    Publisher& operator=(Publisher&& other) noexcept {
        if (this != &other) {
            subscribers_ = std::move(other.subscribers_);
            next_id_ = other.next_id_;
            lock_strategy_ = std::move(other.lock_strategy_);
        }
        return *this;
    }

private:
    friend class Core::Subscription<T>;
    
    void unsubscribe(size_t id) {
        LockGuard guard(lock_strategy_.get());
        subscribers_.erase(
            std::remove_if(subscribers_.begin(), subscribers_.end(),
                [id](const Subscriber& s) { return s.id == id; }),
            subscribers_.end()
        );
    }
};

} // namespace Core