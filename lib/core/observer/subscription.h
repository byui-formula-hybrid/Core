#pragma once

#include <cstddef>

namespace Core {

template<typename T> class Publisher;

/**
 * @brief Subscription handle that automatically unsubscribes when destroyed
 */
template<typename T>
class Subscription {
private:
    Publisher<T>* publisher_;
    size_t id_;
    
public:
    Subscription(Publisher<T>* publisher, size_t id) 
        : publisher_(publisher), id_(id) {}
        
    ~Subscription() {
        if (publisher_) {
            publisher_->unsubscribe(id_);
        }
    }
    
    // Move semantics
    Subscription(Subscription&& other) noexcept 
        : publisher_(other.publisher_), id_(other.id_) {
        other.publisher_ = nullptr;
    }
    
    Subscription& operator=(Subscription&& other) noexcept {
        if (this != &other) {
            if (publisher_) {
                publisher_->unsubscribe(id_);
            }
            publisher_ = other.publisher_;
            id_ = other.id_;
            other.publisher_ = nullptr;
        }
        return *this;
    }
    
    // Disable copy
    Subscription(const Subscription&) = delete;
    Subscription& operator=(const Subscription&) = delete;
};

} // namespace Core