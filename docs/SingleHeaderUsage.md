# Core Library Usage

## Single Header Import

The Core library provides a single header file that imports everything you need:

```cpp
#include <Core.h>  // Everything you need!

using namespace Core;
```

## Benefits of Single Header

### ✅ **Before (Multiple Includes)**
```cpp
#include "Publisher.h"
#include "CurrentValueSubject.h" 
#include "PublisherFactory.h"
#include "LockStrategyFactory.h"
#include "FreeRTOSLockStrategy.h"  // Only on ESP32
// ... and potentially more
```

### 🎯 **After (Single Include)**
```cpp
#include <Core.h>  // Done!
```

## Quick Start Examples

### Simple Publisher
```cpp
#include <Core.h>
using namespace Core;

void setup() {
    auto batteryVoltage = CORE_PUBLISHER(float);
    
    auto subscription = batteryVoltage.sink([](float voltage) {
        Serial.print("Battery: ");
        Serial.println(voltage);
        if (voltage < 3.0f) {
            Serial.println("LOW BATTERY WARNING!");
        }
    });
    
    batteryVoltage.send(3.7f);  // Prints: Battery: 3.7
    batteryVoltage.send(2.8f);  // Prints: Battery: 2.8, LOW BATTERY WARNING!
}
```

### Subject with State
```cpp
#include <Core.h>
using namespace Core;

void setup() {
    auto driverName = CORE_SUBJECT(String, "John");
    
    auto subscription = driverName.sink([](const String& name) {
        Serial.print("Current driver: ");
        Serial.println(name);
    });
    // Immediately prints: "Current driver: John"
    
    driverName.send("Alice");  // Prints: "Current driver: Alice"
    
    String current = driverName.value();  // Get current value
}
```

### Multiple Subscribers
```cpp
#include <Core.h>
using namespace Core;

struct TelemetryData {
    float speed, rpm, temperature;
};

void setup() {
    auto telemetry = CORE_PUBLISHER(TelemetryData);
    
    // Dashboard
    auto dashSub = telemetry.sink([](const TelemetryData& data) {
        Serial.printf("Speed: %.1f | RPM: %.0f | Temp: %.1f°C\n", 
                     data.speed, data.rpm, data.temperature);
    });
    
    // Warnings
    auto warningSub = telemetry.sink([](const TelemetryData& data) {
        if (data.temperature > 95.0f) Serial.println("OVERHEATING!");
        if (data.rpm > 8000.0f) Serial.println("RPM REDLINE!");
    });
    
    // Data logging
    auto logSub = telemetry.sink([](const TelemetryData& data) {
        // Log to SD card
    });
    
    telemetry.send({85.5f, 6500.0f, 87.2f});
    telemetry.send({92.1f, 8200.0f, 98.5f});  // Triggers warnings
}
```

## Convenience Macros

The library provides helpful macros for common patterns:

```cpp
// Instead of: PublisherFactory::create<float>()
auto pub = CORE_PUBLISHER(float);

// Instead of: PublisherFactory::createForTesting<int>()
auto testPub = CORE_TEST_PUBLISHER(int);

// Instead of: CurrentValueSubjectFactory::create<String>("initial")
auto subject = CORE_SUBJECT(String, "initial");

// Instead of: CurrentValueSubjectFactory::createForTesting<bool>()
auto testSubject = CORE_TEST_SUBJECT(bool);
```

## Type Aliases

For even cleaner code:

```cpp
using namespace Core;

ReactivePublisher<float> batteryVoltage = Publishers::create<float>();
ReactiveSubject<String> driverName = Subjects::create<String>("John");
```

## Testing

The library automatically uses appropriate strategies:

```cpp
// In production code (ESP32)
auto publisher = CORE_PUBLISHER(int);  // Uses FreeRTOS mutex

// In test code  
auto publisher = CORE_TEST_PUBLISHER(int);  // Uses mock mutex
```

## Is This Necessary?

**Arguments FOR single header:**
- ✅ Easier for consumers - one include does everything
- ✅ Consistent with popular libraries (like Arduino.h)
- ✅ Reduces chance of missing dependencies
- ✅ Provides convenient macros and aliases
- ✅ Better documentation in one place
- ✅ Version information centralized

**Arguments AGAINST:**
- ❌ Slightly longer compile times (includes unused headers)
- ❌ Less granular control for advanced users

**Recommendation:** Keep both approaches:
- `Core.h` for easy consumption
- Individual headers for advanced users who want granular control

This gives the best of both worlds! 🎉