#include "../mocks/Arduino.h"
#include "../../lib/core/Publisher.h"

using namespace Core;

/**
 * @brief Examples showing how much simpler the new Publisher API is
 */

// Example 1: Simple event publishing
void example_simple_publisher() {
    // Create a publisher for battery voltage
    Publisher<float> batteryVoltage;
    
    // Subscribe with a lambda - subscription automatically managed
    auto subscription = batteryVoltage.sink([](float voltage) {
        Serial.print("Battery voltage: ");
        Serial.println(voltage);
        
        if (voltage < 3.0f) {
            Serial.println("Low battery warning!");
        }
    });
    
    // Send values
    batteryVoltage.send(3.7f);  // Prints: Battery voltage: 3.7
    batteryVoltage.send(2.8f);  // Prints: Battery voltage: 2.8, Low battery warning!
    
    // subscription automatically unsubscribes when it goes out of scope
}

// Example 2: Multiple subscribers
void example_multiple_subscribers() {
    Publisher<int> speedPublisher;
    
    // Display subscription
    auto displaySub = speedPublisher.sink([](int speed) {
        Serial.print("Speed: ");
        Serial.print(speed);
        Serial.println(" km/h");
    });
    
    // Data logging subscription
    auto logSub = speedPublisher.sink([](int speed) {
        // Log to SD card or send to telemetry
        Serial.print("LOG: ");
        Serial.println(speed);
    });
    
    // Safety check subscription
    auto safetySub = speedPublisher.sink([](int speed) {
        if (speed > 100) {
            Serial.println("SPEED LIMIT EXCEEDED!");
        }
    });
    
    speedPublisher.send(85);   // All three callbacks triggered
    speedPublisher.send(120);  // All three + safety warning
}

// Example 3: CurrentValueSubject (holds state)
void example_current_value_subject() {
    // Create with initial value
    CurrentValueSubject<String> driverName("John");
    
    // New subscribers immediately get current value
    auto subscription = driverName.sink([](const String& name) {
        Serial.print("Current driver: ");
        Serial.println(name);
    });
    // Immediately prints: "Current driver: John"
    
    // Update the value
    driverName.send("Alice");  // Prints: "Current driver: Alice"
    
    // Access current value directly
    Serial.print("Driver is: ");
    Serial.println(driverName.value());  // Prints: "Driver is: Alice"
}

// Example 4: Racing car telemetry system
struct TelemetryData {
    float speed;
    float rpm;
    float temperature;
    int gear;
};

void example_racing_telemetry() {
    Publisher<TelemetryData> telemetry;
    
    // Dashboard display
    auto dashboardSub = telemetry.sink([](const TelemetryData& data) {
        Serial.printf("Speed: %.1f | RPM: %.0f | Temp: %.1f°C | Gear: %d\n",
                     data.speed, data.rpm, data.temperature, data.gear);
    });
    
    // Performance monitoring
    auto perfSub = telemetry.sink([](const TelemetryData& data) {
        if (data.temperature > 95.0f) {
            Serial.println("ENGINE OVERHEATING!");
        }
        if (data.rpm > 8000) {
            Serial.println("RPM REDLINE!");
        }
    });
    
    // Send telemetry data
    telemetry.send({85.5f, 6500.0f, 87.2f, 4});
    telemetry.send({92.1f, 8200.0f, 98.5f, 5});  // Triggers warnings
}

// Example 5: Automatic subscription management
class Dashboard {
private:
    Publisher<float>::Subscription speedSub_;
    Publisher<int>::Subscription gearSub_;
    
public:
    Dashboard(Publisher<float>& speedPub, Publisher<int>& gearPub) {
        // Subscriptions are stored as members
        speedSub_ = speedPub.sink([this](float speed) {
            updateSpeedDisplay(speed);
        });
        
        gearSub_ = gearPub.sink([this](int gear) {
            updateGearDisplay(gear);
        });
    }
    
    // Destructor automatically unsubscribes
    ~Dashboard() = default;
    
private:
    void updateSpeedDisplay(float speed) {
        Serial.print("Dashboard Speed: ");
        Serial.println(speed);
    }
    
    void updateGearDisplay(int gear) {
        Serial.print("Dashboard Gear: ");
        Serial.println(gear);
    }
};

void example_automatic_cleanup() {
    Publisher<float> speedPub;
    Publisher<int> gearPub;
    
    {
        Dashboard dashboard(speedPub, gearPub);
        
        speedPub.send(75.0f);  // Dashboard updates
        gearPub.send(3);       // Dashboard updates
        
    } // Dashboard destructor called, subscriptions automatically cleaned up
    
    speedPub.send(80.0f);  // No output - dashboard is gone
}