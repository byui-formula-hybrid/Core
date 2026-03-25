#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 Core Library - Race Car");
    Serial.println("Ready to run any module...");
}

void loop() {
    // Main loop - modules will be added here
    delay(1000);
}