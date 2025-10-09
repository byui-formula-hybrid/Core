#include <Core.h>  // Single include for everything!

using namespace Core;

/**
 * @brief Racing car telemetry example using the Core library
 */

struct TelemetryData {
    float speed;      // km/h
    float rpm;        // RPM
    float temperature; // °C
    int gear;         // Current gear
    float fuel;       // Fuel level %
};

struct WarningData {
    String message;
    int severity;     // 1=info, 2=warning, 3=critical
};

void setup() {
    Serial.begin(115200);
    
    // Create publishers using the simple macros
    auto telemetryPublisher = CORE_PUBLISHER(TelemetryData);
    auto warningPublisher = CORE_PUBLISHER(WarningData);
    
    // Create subjects to hold current state
    auto currentDriver = CORE_SUBJECT(String, "John Doe");
    auto currentLap = CORE_SUBJECT(int, 1);
    
    // Dashboard subscription
    auto dashboardSub = telemetryPublisher.sink([](const TelemetryData& data) {
        Serial.printf("🏎️  Speed: %.1f km/h | RPM: %.0f | Gear: %d | Temp: %.1f°C | Fuel: %.1f%%\n",
                     data.speed, data.rpm, data.gear, data.temperature, data.fuel);
    });
    
    // Performance monitoring subscription  
    auto perfSub = telemetryPublisher.sink([&warningPublisher](const TelemetryData& data) {
        if (data.temperature > 95.0f) {
            warningPublisher.send({"ENGINE OVERHEATING!", 3});
        }
        if (data.rpm > 8000.0f) {
            warningPublisher.send({"RPM REDLINE!", 2});
        }
        if (data.fuel < 10.0f) {
            warningPublisher.send({"LOW FUEL!", 2});
        }
        if (data.speed > 120.0f) {
            warningPublisher.send({"SPEED LIMIT EXCEEDED!", 2});
        }
    });
    
    // Data logging subscription
    auto logSub = telemetryPublisher.sink([](const TelemetryData& data) {
        // Log to SD card or send to pit crew
        Serial.printf("LOG: %.1f,%.0f,%d,%.1f,%.1f\n", 
                     data.speed, data.rpm, data.gear, data.temperature, data.fuel);
    });
    
    // Warning display subscription
    auto warningSub = warningPublisher.sink([](const WarningData& warning) {
        String prefix = warning.severity == 3 ? "🚨 CRITICAL" : 
                       warning.severity == 2 ? "⚠️  WARNING" : "ℹ️  INFO";
        Serial.printf("%s: %s\n", prefix.c_str(), warning.message.c_str());
    });
    
    // Driver change notifications
    auto driverSub = currentDriver.sink([](const String& driver) {
        Serial.printf("👤 Driver: %s\n", driver.c_str());
    });
    // Immediately prints: "👤 Driver: John Doe"
    
    // Lap change notifications
    auto lapSub = currentLap.sink([](int lap) {
        Serial.printf("🏁 Lap: %d\n", lap);
    });
    // Immediately prints: "🏁 Lap: 1"
    
    // Simulate some racing data
    Serial.println("\n🏁 Race Start!\n");
    
    // Lap 1 - conservative start
    telemetryPublisher.send({65.5f, 5500.0f, 78.2f, 3, 98.5f});
    delay(1000);
    
    telemetryPublisher.send({85.3f, 6800.0f, 82.1f, 4, 97.8f});
    delay(1000);
    
    // Push harder - trigger some warnings
    telemetryPublisher.send({125.7f, 8200.0f, 96.5f, 5, 95.2f});
    delay(1000);
    
    // Driver change
    currentDriver.send("Alice Smith");
    delay(500);
    
    // New lap
    currentLap.send(2);
    delay(500);
    
    // Critical situation
    telemetryPublisher.send({110.2f, 7900.0f, 98.8f, 5, 8.5f});
    delay(1000);
    
    Serial.println("\n🏁 Race simulation complete!");
}

void loop() {
    // Main loop can be empty for this example
    delay(1000);
}

/**
 * Example output:
 * 
 * 👤 Driver: John Doe
 * 🏁 Lap: 1
 * 
 * 🏁 Race Start!
 * 
 * 🏎️  Speed: 65.5 km/h | RPM: 5500 | Gear: 3 | Temp: 78.2°C | Fuel: 98.5%
 * LOG: 65.5,5500,3,78.2,98.5
 * 🏎️  Speed: 85.3 km/h | RPM: 6800 | Gear: 4 | Temp: 82.1°C | Fuel: 97.8%
 * LOG: 85.3,6800,4,82.1,97.8
 * 🏎️  Speed: 125.7 km/h | RPM: 8200 | Gear: 5 | Temp: 96.5°C | Fuel: 95.2%
 * LOG: 125.7,8200,5,96.5,95.2
 * ⚠️  WARNING: SPEED LIMIT EXCEEDED!
 * 🚨 CRITICAL: RPM REDLINE!
 * 🚨 CRITICAL: ENGINE OVERHEATING!
 * 👤 Driver: Alice Smith
 * 🏁 Lap: 2
 * 🏎️  Speed: 110.2 km/h | RPM: 7900 | Gear: 5 | Temp: 98.8°C | Fuel: 8.5%
 * LOG: 110.2,7900,5,98.8,8.5
 * 🚨 CRITICAL: ENGINE OVERHEATING!
 * ⚠️  WARNING: LOW FUEL!
 * 
 * 🏁 Race simulation complete!
 */