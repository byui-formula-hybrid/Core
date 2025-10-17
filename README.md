# ESP32 Race Car Core Library

[![CI Pipeline](https://github.com/byui-formula-hybrid/Core/actions/workflows/ci.yml/badge.svg)](https://github.com/byui-formula-hybrid/Core/actions/workflows/ci.yml)

This is a PlatformIO-based ESP32 project for a race car with multiple modular components. Each component is implemented as a separate library for clean organization and easy testing.

## Project Structure

```
Core/
├── .github/                   # GitHub Actions workflows and templates
├── .scripts/                  # Automated setup, build, and test scripts
├── docs/                      # Project documentation and design specs
├── include/                   # Shared headers and global configuration
├── lib/                       # Custom race car module libraries
│   ├── battery/               # Battery monitoring and management
│   ├── can/                   # CAN bus communication protocol
│   ├── core/                  # Core system functionality and reactive patterns
│   ├── dash/                  # Dashboard and display interface
│   ├── imd/                   # Insulation monitoring device
│   ├── inverter/              # Motor inverter control
│   ├── pedals/                # Accelerator and brake pedal input
│   └── pump/                  # Cooling pump control
├── src/                       # Main ESP32 application source code
└── test/                      # Unit tests and test helpers
    ├── helpers/               # Test factory implementations
    ├── mocks/                 # Mock objects and strategies
    ├── test_battery/          # Battery module tests
    ├── test_can/              # CAN module tests
    ├── test_core/             # Core module tests
    ├── test_dash/             # Dashboard module tests
    ├── test_imd/              # IMD module tests
    ├── test_inverter/         # Inverter module tests
    ├── test_pedals/           # Pedals module tests
    └── test_pump/             # Pump module tests
```

## Getting Started

> **💡 Important:** This project includes automated scripts that handle all setup, building, and testing. **Use the provided scripts instead of manual commands** for the best experience and to avoid common setup issues.


### Quick Start (Recommended)

**New to the project? Start here:**


#### For Linux/macOS
```bash
# 1. Clone the repository
git clone https://github.com/byui-formula-hybrid/Core.git
cd Core

# 2. Run the automated setup script
./.scripts/install.sh

# 3. Test that everything works
./.scripts/test.sh

# 4. Build for ESP32
./.scripts/build.sh
```

#### For Windows
> **🪟 Windows Notice:**
>
> You have two options for running project scripts on Windows:
>
> **Option 1: Use PowerShell scripts**
>
> Open PowerShell and run:
> ```powershell
> # 1. Clone the repository
> git clone https://github.com/byui-formula-hybrid/Core.git
> cd Core
>
> # 2. Run the automated setup script
> .\.scripts\powershell\install.ps1
>
> # 3. Test that everything works
> .\.scripts\powershell\test.ps1
>
> # 4. Build for ESP32
> .\.scripts\powershell\build.ps1
> ```
>
> **Option 2: Use Git Bash and bash scripts**
>
> 1. Open PowerShell and run:
>    ```powershell
>    .\.scripts\setup-gitbash.ps1
>    ```
> 2. After setup, open a Git Bash terminal (not PowerShell or CMD).
> 3. Run all project scripts (e.g., `build.sh`, `test.sh`, `install.sh`) in Git Bash using the same syntax as on macOS/Linux:
>    ```bash
>    # 1. Clone the repository
>    git clone https://github.com/byui-formula-hybrid/Core.git
>    cd Core
>
>    # 2. Run the automated setup script
>    ./.scripts/install.sh
>
>    # 3. Test that everything works
>    ./.scripts/test.sh
>
>    # 4. Build for ESP32
>    ./.scripts/build.sh
>    ```
>
> This ensures all scripts work correctly and avoids shell compatibility issues.

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

## CI/CD Pipeline

This project uses GitHub Actions for continuous integration with smart safeguards and efficiency optimizations.

### 🚀 **Pipeline Features**
- **Manual Trigger**: Run on-demand from GitHub Actions tab
- **Smart Path Filtering**: Skips runs for documentation-only changes  
- **Timeout Protection**: 10min tests, 15min builds, never hangs
- **Multi-Runner Support**: Ubuntu (default), macOS, Windows fallback
- **Auto-Cancel**: New commits stop previous runs, saving CI minutes
- **Artifact Storage**: ESP32 firmware binaries saved for 7 days

### 🧪 **What Gets Tested**
- **Native Tests**: All unit tests using Unity framework
- **ESP32 Build**: Full firmware compilation and verification
- **Cross-Platform**: Optional validation on multiple OS types

### 📊 **Pipeline Triggers**
- ✅ **Pull Requests** to `main` (automatic)
- ✅ **Pushes** to `main` (releases)  
- ✅ **Manual Runs** (on-demand via GitHub UI)
- ❌ **Documentation** changes only (skipped)

### 🛠️ **Usage**
```bash
# Manual trigger via GitHub:
# Actions → CI Pipeline → Run workflow
```

**📚 Detailed Guide:** [`.github/workflows/README.md`](.github/workflows/README.md)

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
