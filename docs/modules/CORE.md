# Core Module Documentation
<!-- Reactive programming library for ESP32 Formula Hybrid systems -->

## Scope
<!-- Public API documentation for the Core reactive programming library. Internal implementation details like FreeRTOSLockStrategy are not covered. -->

This document covers the public API of the Core reactive programming library designed for ESP32-based Formula Hybrid vehicle systems. The library provides Swift Combine-inspired reactive programming patterns with thread-safe publishers and subscribers.

## Public Components

### Publisher<T>
The core reactive component for broadcasting values to multiple subscribers.

#### Basic Usage
```cpp
#include <Core.h>
using namespace Core;

// Create a publisher for CAN messages
Publisher<CANMessage> can_publisher;

// Subscribe to messages
auto subscription = can_publisher.sink([](const CANMessage& msg) {
    Serial.print("CAN ID: ");
    Serial.println(msg.id);
});

// Broadcast to all subscribers
CANMessage msg = {0x123, {0x01, 0x02, 0x03}};
can_publisher.send(msg);
```

#### Thread-Safe Usage (ESP32)
```cpp
// Create with FreeRTOS mutex for thread safety
auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
Publisher<float> sensor_data(std::move(lockStrategy));

// Safe to use from multiple tasks
auto logger_sub = sensor_data.sink([](float value) {
    logger.record(value);
});

auto display_sub = sensor_data.sink([](float value) {
    display.update(value);
});
```

#### Methods
- `Subscription sink(std::function<void(const T&)> callback)` - Subscribe to value changes
- `void send(const T& value)` - Broadcast value to all subscribers
- `size_t subscriberCount() const` - Get number of active subscribers

### CurrentValueSubject<T>
A stateful publisher that holds the current value and immediately delivers it to new subscribers.

#### Basic Usage
```cpp
// Create with initial value
CurrentValueSubject<bool> engine_running(false);

// New subscribers immediately get current value (false)
auto dashboard_sub = engine_running.sink([](bool running) {
    dashboard.setEngineStatus(running);
});

// Update state - all subscribers notified
engine_running.send(true);
```

#### State Management
```cpp
CurrentValueSubject<BatteryLevel> battery_status;

// Check if has value
if (battery_status.hasValue()) {
    BatteryLevel current = battery_status.value();
    Serial.print("Battery: ");
    Serial.println(current.percentage);
}

// Set initial state
battery_status.send({85, GOOD});
```

#### Methods
- All Publisher<T> methods, plus:
- `const T& value() const` - Get current stored value
- `bool hasValue() const` - Check if value has been set

### Subscription
RAII subscription handle that automatically unsubscribes when destroyed.

#### Usage
```cpp
{
    auto subscription = publisher.sink([](int value) {
        // Handle value
    });
    
    // Subscription is active here
    
} // Automatic unsubscription when scope exits

// Subscription is now inactive
```

#### Manual Control
```cpp
auto subscription = publisher.sink(callback);

// Manually unsubscribe
subscription.unsubscribe();

// Check if still active
if (subscription.isActive()) {
    // Still subscribed
}
```

### Factory Interfaces
Generic interfaces for creating reactive components with custom implementations.

#### iPublisherFactory<T>
Abstract interface for creating Publishers of type T.

```cpp
// Implement for your platform
class ESP32CANFactory : public iPublisherFactory<CANMessage> {
public:
    Publisher<CANMessage> create() override {
        auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
        return Publisher<CANMessage>(std::move(lockStrategy));
    }
};

// Use the factory
ESP32CANFactory canFactory;
auto canPublisher = canFactory.create();
```

#### iCurrentValueSubjectFactory<T>
Abstract interface for creating CurrentValueSubjects of type T.

```cpp
// Implement for your platform  
class ESP32BatteryFactory : public iCurrentValueSubjectFactory<float> {
public:
    CurrentValueSubject<float> create() override {
        auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
        return CurrentValueSubject<float>(std::move(lockStrategy));
    }
    
    CurrentValueSubject<float> create(const float& initial_value) override {
        auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
        return CurrentValueSubject<float>(initial_value, std::move(lockStrategy));
    }
};

// Use the factory
ESP32BatteryFactory batteryFactory;
auto batteryLevel = batteryFactory.create(85.0f); // Start at 85%
```

### Factory Classes
Convenient factory methods for creating publishers with appropriate lock strategies.

#### PublisherFactory
```cpp
// Create thread-safe publisher
auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
auto publisher = PublisherFactory::create<CANMessage>(std::move(lockStrategy));

// For single-threaded use, use direct constructor
Publisher<int> simple_publisher; // No thread safety
```

#### CurrentValueSubjectFactory
```cpp
// Thread-safe with initial value
auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
auto subject = CurrentValueSubjectFactory::create<float>(25.0f, std::move(lockStrategy));

// Single-threaded with initial value
CurrentValueSubject<bool> flag(true); // Direct constructor
```

## Practical Examples

### CAN Bus Message Processing
```cpp
class CANController {
private:
    Publisher<CANMessage> can_stream;
    std::vector<Subscription> subscriptions;
    
public:
    CANController() {
        // Thread-safe for FreeRTOS tasks
        auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
        can_stream = Publisher<CANMessage>(std::move(lockStrategy));
    }
    
    void onCANReceived(const CANMessage& msg) {
        can_stream.send(msg); // Broadcast to all modules
    }
    
    void registerBrainModule() {
        auto sub = can_stream.sink([](const CANMessage& msg) {
            if (msg.id >= 0x100 && msg.id <= 0x1FF) {
                brain_module.process(msg);
            }
        });
        subscriptions.push_back(std::move(sub));
    }
    
    void registerLogger() {
        auto sub = can_stream.sink([](const CANMessage& msg) {
            logger.logCAN(msg.id, msg.data, msg.length);
        });
        subscriptions.push_back(std::move(sub));
    }
};
```

### Vehicle State Management
```cpp
class VehicleState {
private:
    CurrentValueSubject<VehicleMode> current_mode;
    CurrentValueSubject<float> speed;
    CurrentValueSubject<BatteryInfo> battery;
    
public:
    VehicleState() : current_mode(ECO_MODE), speed(0.0f) {
        // Battery starts without value until first reading
    }
    
    // State accessors
    VehicleMode getMode() const { return current_mode.value(); }
    float getCurrentSpeed() const { return speed.value(); }
    
    // State updates
    void setMode(VehicleMode mode) { current_mode.send(mode); }
    void updateSpeed(float new_speed) { speed.send(new_speed); }
    void updateBattery(const BatteryInfo& info) { battery.send(info); }
    
    // Subscribe to state changes
    Subscription onModeChange(std::function<void(VehicleMode)> callback) {
        return current_mode.sink(callback);
    }
    
    Subscription onSpeedChange(std::function<void(float)> callback) {
        return speed.sink(callback);
    }
};
```

### Multi-Module System
```cpp
// Global vehicle state
VehicleState vehicle_state;

// Dashboard module
class Dashboard {
    std::vector<Subscription> subscriptions;
    
public:
    void initialize() {
        // Subscribe to all relevant state
        subscriptions.push_back(
            vehicle_state.onSpeedChange([this](float speed) {
                display.showSpeed(speed);
            })
        );
        
        subscriptions.push_back(
            vehicle_state.onModeChange([this](VehicleMode mode) {
                display.showMode(mode);
            })
        );
    }
};

// Safety module
class SafetySystem {
    std::vector<Subscription> subscriptions;
    
public:
    void initialize() {
        subscriptions.push_back(
            vehicle_state.onSpeedChange([this](float speed) {
                if (speed > MAX_SAFE_SPEED) {
                    triggerSpeedAlert();
                }
            })
        );
    }
};
```

## Best Practices

### Memory Management
- Use RAII subscriptions - they automatically clean up
- Store subscriptions in containers to manage lifetime
- Prefer direct constructors for single-threaded scenarios

### Thread Safety
- Always use `FreeRTOSLockStrategy` for multi-task environments on ESP32
- Use direct constructors only for single-threaded code
- Be consistent within each module about locking strategy

### Performance
- Publishers are lightweight - create them freely
- Subscriptions have minimal overhead
- CurrentValueSubject has small memory footprint suitable for embedded systems

### Architecture
- Use Publisher for event streams (CAN messages, sensor readings)
- Use CurrentValueSubject for state that new subscribers need immediately
- Keep callback functions lightweight to avoid blocking other subscribers

## Resources
- [Usage Examples](../Usage.md) - More detailed usage scenarios
- [API Reference](../API.md) - Complete method documentation
- [Testing Guide](../Testing.md) - How to test reactive code