#include "../../lib/core/Publisher.h"
#include "../../lib/core/FreeRTOSLockStrategy.h"

using namespace Core;

/**
 * @brief Examples showing clean dependency injection usage
 */

// Example 1: Default strategy (automatic platform detection)
void example_default_strategy() {
    // Uses MockLockStrategy in tests, FreeRTOSLockStrategy on ESP32
    Publisher<float> batteryVoltage;
    
    auto subscription = batteryVoltage.sink([](float voltage) {
        Serial.print("Battery: ");
        Serial.println(voltage);
    });
    
    batteryVoltage.send(3.7f);
}

// Example 2: Explicit dependency injection
void example_explicit_injection() {
    // Inject specific lock strategy
    auto lock_strategy = std::make_unique<FreeRTOSLockStrategy>();
    Publisher<int> speedPublisher(std::move(lock_strategy));
    
    auto subscription = speedPublisher.sink([](int speed) {
        Serial.print("Speed: ");
        Serial.println(speed);
    });
    
    speedPublisher.send(85);
}

// Example 3: Testing with mock injection
void example_test_injection() {
    // In tests, inject mock to verify behavior
    auto mock_lock = std::make_unique<MockLockStrategy>();
    MockLockStrategy* mock_ptr = mock_lock.get();
    
    Publisher<String> messagePublisher(std::move(mock_lock));
    
    auto subscription = messagePublisher.sink([](const String& msg) {
        Serial.println(msg);
    });
    
    messagePublisher.send("Hello World");
    
    // Verify locking behavior in tests
    // mock_ptr->lock_count should be > 0
    // mock_ptr->unlock_count should equal lock_count
}

// Example 4: Factory pattern for different strategies
class PublisherFactory {
public:
    template<typename T>
    static Publisher<T> createForTesting() {
        return Publisher<T>(LockStrategyFactory::createMock());
    }
    
    template<typename T>
    static Publisher<T> createForProduction() {
        return Publisher<T>(LockStrategyFactory::createDefault());
    }
};

void example_factory_usage() {
    // Clean factory usage
    auto testPublisher = PublisherFactory::createForTesting<int>();
    auto prodPublisher = PublisherFactory::createForProduction<int>();
    
    // Both work the same way
    auto sub1 = testPublisher.sink([](int x) { /* test logic */ });
    auto sub2 = prodPublisher.sink([](int x) { /* prod logic */ });
    
    testPublisher.send(42);
    prodPublisher.send(42);
}

// Example 5: Racing car telemetry with dependency injection
class TelemetrySystem {
private:
    Publisher<float> speed_publisher_;
    Publisher<float> rpm_publisher_;
    Publisher<float> temp_publisher_;
    
public:
    TelemetrySystem(std::unique_ptr<LockStrategy> lock_strategy) 
        : speed_publisher_(std::unique_ptr<LockStrategy>(lock_strategy.get())),
          rpm_publisher_(std::unique_ptr<LockStrategy>(lock_strategy.get())),
          temp_publisher_(std::move(lock_strategy)) {
        // Note: In real usage, you'd create separate strategies for each publisher
        // This is just an example
    }
    
    // Alternative: inject factory
    TelemetrySystem() 
        : speed_publisher_(LockStrategyFactory::createDefault()),
          rpm_publisher_(LockStrategyFactory::createDefault()),
          temp_publisher_(LockStrategyFactory::createDefault()) {
    }
    
    Publisher<float>& speed() { return speed_publisher_; }
    Publisher<float>& rpm() { return rpm_publisher_; }
    Publisher<float>& temperature() { return temp_publisher_; }
};

void example_telemetry_system() {
    TelemetrySystem telemetry; // Uses factory internally
    
    // Subscribe to different data streams
    auto speedSub = telemetry.speed().sink([](float speed) {
        Serial.print("Speed: ");
        Serial.println(speed);
    });
    
    auto rpmSub = telemetry.rpm().sink([](float rpm) {
        if (rpm > 8000) Serial.println("RPM REDLINE!");
    });
    
    auto tempSub = telemetry.temperature().sink([](float temp) {
        if (temp > 95.0f) Serial.println("OVERHEATING!");
    });
    
    // Send data
    telemetry.speed().send(85.5f);
    telemetry.rpm().send(8200.0f);  // Triggers warning
    telemetry.temperature().send(87.2f);
}