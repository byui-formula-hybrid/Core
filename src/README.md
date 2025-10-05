# src/ Directory

## Purpose
This directory contains the **main application code** that runs on the ESP32.

## What Goes Here
- **`main.cpp`** - The entry point of your ESP32 application
- **Application-level logic** - Code that coordinates and uses the modules
- **Setup and loop functions** - Standard Arduino/ESP32 structure

## What Does NOT Go Here
- ❌ Reusable libraries (those go in `lib/`)
- ❌ Configuration headers (those go in `include/`)
- ❌ Test files (those go in `test/`)

## Example Structure
```
src/
└── main.cpp              # ESP32 application entry point
```

## Key Points
- **Single responsibility** - This is just the application that uses your modules
- **Minimal code** - Most logic should be in reusable modules in `lib/`
- **Coordination** - This is where you decide which modules to use and how they work together

## Example Pattern
```cpp
#include <Arduino.h>
#include "PedalsModule.h"
#include "CoreModule.h"

PedalsModule pedals;
CoreModule core;

void setup() {
    Serial.begin(115200);
    
    // Initialize modules
    pedals.begin();
    core.begin();
}

void loop() {
    // Update modules
    pedals.update();
    core.update();
    
    delay(10);
}
```