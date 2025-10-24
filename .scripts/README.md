# ESP32 Formula Hybrid Core Library - Scripts

This folder contains cross-platform scripts for managing the project, except Windows, see the powershell folder for that or use the Git Bash installation for Windows then these apply.

## Scripts

### `install.sh` - Install Dependencies
Installs all required software for development:
- Python 3
- Git
- PlatformIO Core
- VS Code (optional)
- Project dependencies

**Usage:**
```bash
.scripts/install.sh
```

**Supported Platforms:**
- macOS (via Homebrew)
- Linux (via apt, yum, dnf, pacman)
- Windows (via chocolatey, winget)

### `test.sh` - Run Tests
Runs the native unit test suite with ESP32 mocks.

**Usage:**
```bash
.scripts/test.sh [OPTIONS]

Options:
  -v, --verbose    Show detailed test output
  -f, --filter     Run specific test module
  -h, --help       Show help message
```

**Examples:**
```bash
.scripts/test.sh                          # Run all tests
.scripts/test.sh --verbose                # Verbose output
.scripts/test.sh --filter test_core_module # Run specific test
```

### `build.sh` - Build Project
Builds the project for different target environments.

**Usage:**
```bash
.scripts/build.sh [OPTIONS]

Options:
  -e, --env ENVIRONMENT    Build environment (esp32dev, native, wokwi)
  -c, --clean              Clean build files first
  -u, --upload             Upload to device after building
  -m, --monitor            Start serial monitor after upload
  -h, --help               Show help message
```

**Examples:**
```bash
.scripts/build.sh                       # Build for ESP32
.scripts/build.sh --clean               # Clean build
.scripts/build.sh --upload              # Build and upload
.scripts/build.sh --upload --monitor    # Build, upload, and monitor
.scripts/build.sh --env native          # Build native version
.scripts/build.sh --env wokwi           # Build for Wokwi simulation
```

### `uninstall.sh` - Remove Dependencies
Removes PlatformIO and project build files (keeps Python, Git, VS Code).

**Usage:**
```bash
.scripts/uninstall.sh
```

## Shared Functions

### `shared.sh` - Common Functions
Contains shared functions used by other scripts:
- OS detection
- Package manager detection
- Colored output
- Dependency installation helpers

**Note:** This file is sourced by other scripts, not executed directly.

## Quick Start

1. **Initial setup:**
   ```bash
   .scripts/install.sh
   ```

2. **Run tests:**
   ```bash
   .scripts/test.sh
   ```

3. **Build for ESP32:**
   ```bash
   .scripts/build.sh
   ```

4. **Build and upload:**
   ```bash
   .scripts/build.sh --upload --monitor
   ```

## Cross-Platform Support

These scripts work on:
- **macOS** - Uses Homebrew for package management
- **Linux** - Supports apt (Ubuntu/Debian), yum/dnf (RHEL/Fedora), pacman (Arch)
- **Windows** - Supports chocolatey and winget package managers

The scripts automatically detect your operating system and use the appropriate package manager.