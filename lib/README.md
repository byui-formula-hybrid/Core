# lib/ Directory

## Purpose
This directory contains **project-specific libraries** - the reusable modules for your race car system.

## What Goes Here
- **Custom modules** - Each race car component (pedals, battery, etc.)
- **Self-contained libraries** - Each module has its own folder
- **`.h` and `.cpp` files together** - Interface and implementation in same folder

## What Does NOT Go Here
- ❌ Main application code (that goes in `src/`)
- ❌ Global configuration (that goes in `include/`)
- ❌ External libraries (PlatformIO manages those automatically)

## Structure Pattern
Each module follows this pattern:
```
lib/
├── ModuleName/
│   ├── ModuleName.h           # Public interface (what the module provides)
│   └── ModuleName.cpp         # Implementation (how it works)
└── AnotherModule/
    ├── AnotherModule.h
    └── AnotherModule.cpp
```

## Current Modules
- **`CanModule/`** - CAN bus communication logic
- **`CoreModule/`** - Main vehicle controller logic  
- **`DashModule/`** - Dashboard display logic
- **`InverterModule/`** - Motor controller interface logic
- **`PumpModule/`** - Cooling pump controller logic
- **`PedalsModule/`** - Throttle/brake pedal processing logic
- **`BatteryModule/`** - Battery management logic
- **`ImdModule/`** - Insulation monitoring logic

## Key Benefits
- ✅ **Self-contained** - Each module is independent
- ✅ **Reusable** - Can be copied to other projects
- ✅ **Testable** - Each module can be unit tested separately
- ✅ **Clear ownership** - Each .h/.cpp pair belongs together