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
│   ├── can/                   # CAN bus communication module
│   ├── core/                  # Core system functionality
│   ├── dash/                  # Dashboard and display module
│   ├── inverter/              # Motor inverter control module
│   ├── pump/                  # Cooling pump control module
│   ├── pedals/                # Accelerator/brake pedal module
│   ├── battery/               # Battery monitoring module
│   └── imd/                   # Insulation monitoring device module
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
│   ├── test_imd/              # IMD module tests
│   └── mocks/                 # Mock objects for testing
└── docs/                      # Project documentation
    ├── CANbus.md              # CAN bus protocol documentation
    ├── Design.md              # System design and architecture
    └── README.md              # Documentation overview
```

## Getting Started

> **💡 Important:** This project includes automated scripts that handle all setup, building, and testing. **Use the provided scripts instead of manual commands** for the best experience and to avoid common setup issues.

### Quick Start (Recommended)

**New to the project? Start here:**

```bash
# 1. Clone the repository
git clone <repository-url>
cd Core

# 2. Run the automated setup script
./.scripts/install.sh

# 3. Test that everything works
./.scripts/test.sh

# 4. Build for ESP32
./.scripts/build.sh
```

The setup script will automatically:
- ✅ Detect your operating system (macOS, Linux, Windows)
- ✅ Install required dependencies (Python, PlatformIO, etc.)
- ✅ Configure your development environment
- ✅ Verify everything is working

### All Available Scripts

| Script | Purpose | Example Usage |
|--------|---------|---------------|
| `./.scripts/install.sh` | **Setup everything** | `./.scripts/install.sh` |
| `./.scripts/test.sh` | **Run unit tests** | `./.scripts/test.sh --verbose` |
| `./.scripts/build.sh` | **Build project** | `./.scripts/build.sh --upload` |
| `./.scripts/uninstall.sh` | **Clean up** | `./.scripts/uninstall.sh` |

📚 **For detailed script options:** See [`.scripts/README.md`](.scripts/README.md)

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
