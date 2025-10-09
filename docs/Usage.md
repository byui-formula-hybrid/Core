# Core Library Usage Examples

## For ESP32 Production Code

```cpp
#include <Core.h>
using namespace Core;

void setup() {
    // Create thread-safe publisher for ESP32
    auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
    auto sensorData = PublisherFactory::create<float>(std::move(lockStrategy));
    
    // Subscribe to sensor updates
    auto subscription = sensorData.sink([](float value) {
        Serial.print("Sensor reading: ");
        Serial.println(value);
    });
    
    // In your sensor reading task/interrupt:
    sensorData.send(42.5f);
}
```

## For Single-Threaded Applications

```cpp
#include <Core.h>
using namespace Core;

void loop() {
    // No thread safety needed
    auto eventPublisher = PublisherFactory::createUnsafe<String>();
    
    auto subscription = eventPublisher.sink([](const String& event) {
        Serial.println("Event: " + event);
    });
    
    eventPublisher.send("Button pressed");
}
```

## For Unit Tests

```cpp
#include "../../test/helpers/TestPublisherFactory.h"

void test_my_component() {
    // Use test factory with native lock strategy (real std::mutex)
    auto publisher = TestPublisherFactory::create<int>();
    
    int received = 0;
    auto subscription = publisher.sink([&](int value) {
        received = value;
    });
    
    publisher.send(123);
    TEST_ASSERT_EQUAL(123, received);
    
    // This provides real thread safety using std::mutex
    // Perfect for testing multi-threaded behavior on host platform
}
```

## Current Value Subjects

```cpp
// For stateful data that new subscribers should immediately receive
auto lockStrategy = std::unique_ptr<LockStrategy>(new FreeRTOSLockStrategy());
auto temperature = CurrentValueSubjectFactory::create<float>(
    25.0f, // initial value
    std::move(lockStrategy)
);

// New subscribers immediately get the current value (25.0)
auto subscription = temperature.sink([](float temp) {
    Serial.print("Temperature: ");
    Serial.println(temp);
});

// Update the value
temperature.send(26.5f);

// For single-threaded scenarios
auto unsafeSubject = CurrentValueSubjectFactory::createUnsafe<int>(42);
```