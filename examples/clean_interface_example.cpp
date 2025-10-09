#include <Core.h>  // Single clean include

using namespace Core;

/**
 * @brief Clean example using simplified Core library interface
 */

struct CarData {
    float speed;
    float rpm;
    float temperature;
};

void setup() {
    Serial.begin(115200);
    
    // Clean, simple API - no macros, no complexity
    auto speedPublisher = PublisherFactory::create<float>();
    auto carDataPublisher = PublisherFactory::create<CarData>();
    auto driverSubject = CurrentValueSubjectFactory::create<String>("Unknown");
    
    // Subscribe to speed updates
    auto speedSub = speedPublisher.sink([](float speed) {
        Serial.print("Speed: ");
        Serial.print(speed);
        Serial.println(" km/h");
        
        if (speed > 100.0f) {
            Serial.println("⚠️ Speed limit exceeded!");
        }
    });
    
    // Subscribe to car data
    auto carSub = carDataPublisher.sink([](const CarData& data) {
        Serial.printf("🏎️ Speed: %.1f | RPM: %.0f | Temp: %.1f°C\n", 
                     data.speed, data.rpm, data.temperature);
                     
        if (data.temperature > 95.0f) {
            Serial.println("🚨 Engine overheating!");
        }
    });
    
    // Subscribe to driver changes (gets current value immediately)
    auto driverSub = driverSubject.sink([](const String& driver) {
        Serial.print("👤 Driver: ");
        Serial.println(driver);
    });
    // Immediately prints: "👤 Driver: Unknown"
    
    Serial.println("\n🏁 Racing simulation starting...\n");
    
    // Update driver
    driverSubject.send("Alice");
    delay(500);
    
    // Send some speed data
    speedPublisher.send(45.5f);
    speedPublisher.send(78.2f);
    speedPublisher.send(125.7f);  // Triggers warning
    
    // Send car telemetry
    carDataPublisher.send({85.3f, 6500.0f, 87.1f});
    carDataPublisher.send({110.2f, 7800.0f, 96.5f});  // Triggers overheating
    
    Serial.println("\n✅ Simulation complete!");
}

void loop() {
    delay(1000);
}

/**
 * Example output:
 * 
 * 👤 Driver: Unknown
 * 
 * 🏁 Racing simulation starting...
 * 
 * 👤 Driver: Alice
 * Speed: 45.5 km/h
 * Speed: 78.2 km/h
 * Speed: 125.7 km/h
 * ⚠️ Speed limit exceeded!
 * 🏎️ Speed: 85.3 | RPM: 6500 | Temp: 87.1°C
 * 🏎️ Speed: 110.2 | RPM: 7800 | Temp: 96.5°C
 * 🚨 Engine overheating!
 * 
 * ✅ Simulation complete!
 */