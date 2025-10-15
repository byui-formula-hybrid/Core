# include/ Directory

## Purpose
This directory contains **global headers and configuration** that are shared across multiple modules.

## What Goes Here
- **Configuration files** - Constants, pin definitions, thresholds
- **Shared data structures** - Types used by multiple modules
- **Global constants** - Values that multiple modules need
- **System-wide definitions** - Enums, macros, etc.

## What Does NOT Go Here
- ❌ Module-specific headers (those stay with their .cpp files in `lib/`)
- ❌ Implementation files (.cpp) - only headers (.h) go here
- ❌ Main application code (that goes in `src/`)

## Example Structure
```
include/
├── config.h               # Global configuration constants
├── types.h                # Shared data structures and types
├── pins.h                 # Pin definitions for all modules
└── safety.h               # Safety thresholds and fault codes
```

## Current Files
- **`config.h`** - Global configuration constants (CAN speed, timing, etc.)
- **`types.h`** - Shared data structures (PedalData, BatteryData, etc.)

## Usage Pattern
Modules in `lib/` include these headers when they need shared definitions:

```cpp
// In lib/PedalsModule/PedalsModule.h
#pragma once
#include "../../include/config.h"    // Access global config
#include "../../include/types.h"     // Access shared types

class PedalsModule {
    // Can now use constants from config.h and types from types.h
};
```

## What Should Go Here

### config.h
- CAN bus settings (speed, pins)
- Timing constants (heartbeat intervals, timeouts)
- System-wide constants

### types.h  
- Data structures shared between modules
- Enums for states, error codes, etc.
- Type definitions

### pins.h (future)
- Pin assignments for each board/module
- GPIO definitions
- Analog/digital pin mappings

### safety.h (future)
- Safety thresholds (voltage limits, temperature limits)
- Fault codes and severity levels
- Emergency stop conditions

## Key Benefits
- ✅ **Single source of truth** - Configuration in one place
- ✅ **Consistency** - All modules use same constants
- ✅ **Easy to modify** - Change settings in one file
- ✅ **No duplication** - Shared types defined once