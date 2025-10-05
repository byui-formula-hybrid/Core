# ESP32 Race Car Core Library

This is a PlatformIO-based ESP32 project for a race car with multiple modular components. Each component is implemented as a separate library for clean organization and easy testing.

## Project Structure

```
/Users/po/Developer/Core/
├── README.md                  # This file - project overview and setup
├── platformio.ini             # PlatformIO configuration 
├── .gitignore                 # Git ignore patterns for ESP32/embedded development
├── src/                       # Main application source code
│   └── main.cpp               # ESP32 application entry point
├── lib/                       # Custom race car module libraries
│   ├── CanModule/             # CAN bus communication module
│   ├── CoreModule/            # Core system functionality
│   ├── DashModule/            # Dashboard and display module
│   ├── InverterModule/        # Motor inverter control module
│   ├── PumpModule/            # Cooling pump control module
│   ├── PedalsModule/          # Accelerator/brake pedal module
│   ├── BatteryModule/         # Battery monitoring module
│   └── ImdModule/             # Insulation monitoring device module
├── include/                   # Shared headers and configuration
│   ├── config.h               # Global configuration constants
│   └── types.h                # Shared data structures
├── test/                      # Unit tests for each module
│   ├── test_can/              # CAN module tests
│   ├── test_core/             # Core module tests
│   ├── test_dash/             # Dashboard module tests
│   ├── test_inverter/         # Inverter module tests
│   ├── test_pump/             # Pump module tests
│   ├── test_pedals/           # Pedals module tests
│   ├── test_battery/          # Battery module tests
│   └── test_imd/              # IMD module tests
└── docs/                      # Project documentation
    ├── CANbus.md              # CAN bus protocol documentation
    ├── Design.md              # System design and architecture
    └── README.md              # Documentation overview
```

## Getting Started

### Prerequisites
- VS Code with PlatformIO extension
- Python 3.7+ (for PlatformIO tools)

### Setup
1. Clone this repository
2. Open the project folder in VS Code
3. PlatformIO should automatically detect the project
4. Build with: `pio run`
5. Upload to ESP32: `pio run --target upload`

### Testing
Run unit tests for all modules:
```bash
pio test
```

Run tests for a specific module:
```bash
pio test --filter test_can
```

## Module Overview

Each module in the `lib/` directory follows a consistent interface:
- `ModuleName.h` - Header file with class definition
- `ModuleName.cpp` - Implementation file
- `bool begin()` - Initialize the module
- `void update()` - Update module state (call in main loop)

## Contributing

When adding new modules or features:
1. Create the module in `lib/ModuleName/`
2. Add corresponding tests in `test/test_module/`
3. Update documentation in `docs/`
4. Follow the existing naming conventions

## License

This project is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License - see the [LICENSE](LICENSE) file for details.

This means you can:
- ✅ Use this code for educational purposes
- ✅ Share it with other Formula teams
- ✅ Modify and improve it
- ✅ Use it in Formula Hybrid competitions

But you must:
- 📝 Give credit to BYU-I Formula Hybrid
- 🚫 Not use it for commercial purposes
- 🔄 Share any improvements under the same license
