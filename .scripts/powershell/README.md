# ESP32 Formula Hybrid Core Library - Scripts

This folder contains scripts for managing the project on Windows. If you plan to use Git Bash prompt (optional) then use the README in the folder above for bash scripts.

## Scripts

### `install.ps1` - Install Dependencies
Installs all required software for development:
- Python 3
- Git
- PlatformIO Core
- VS Code (optional)
- Project dependencies
- C and C++ libraries

**Usage:**
```powershell
.\.\.scripts\powershell\powershell\install.ps1
```

**Supported Platforms:**
- Windows (via chocolatey, winget)

### `test.ps1` - Run Tests
Runs the native unit test suite with ESP32 mocks.

**Usage:**
```powershell
.\.scripts\powershell\test.ps1 [OPTIONS]

Options:
  -v, --verbose    Show detailed test output
  -f, --filter     Run specific test module
  -h, --help       Show help message
```

**Examples:**
```powershell
.\.scripts\powershell\test.ps1                          # Run all tests
.\.scripts\powershell\test.ps1 --verbose                # Verbose output
.\.scripts\powershell\test.ps1 --filter test_core_module # Run specific test
```

### `build.ps1` - Build Project
Builds the project for different target environments.

**Usage:**
```powershell
.\.scripts\powershell\build.ps1 [OPTIONS]

Options:
  -e, --env ENVIRONMENT    Build environment (esp32dev, native, wokwi)
  -c, --clean              Clean build files first
  -u, --upload             Upload to device after building
  -m, --monitor            Start serial monitor after upload
  -h, --help               Show help message
```

**Examples:**
```powershell
.\.scripts\powershell\build.ps1                       # Build for ESP32
.\.scripts\powershell\build.ps1 --clean               # Clean build
.\.scripts\powershell\build.ps1 --upload              # Build and upload
.\.scripts\powershell\build.ps1 --upload --monitor    # Build, upload, and monitor
.\.scripts\powershell\build.ps1 --env native          # Build native version
.\.scripts\powershell\build.ps1 --env wokwi           # Build for Wokwi simulation
```

### `uninstall.ps1` - Remove Dependencies
Removes PlatformIO and project build files (keeps Python, Git, VS Code).

**Usage:**
```powershell
.\.\.scripts\powershell\powershell\uninstall.ps1
```

## Shared Functions

### `shared.ps1` - Common Functions
Contains shared functions used by other scripts:
- OS detection
- Package manager detection
- Colored output
- Dependency installation helpers

**Note:** This file is sourced by other scripts, not executed directly.

## Quick Start

1. **Initial setup:**
   ```powershell
   .\.\.scripts\powershell\powershell\install.ps1
   ```

2. **Run tests:**
   ```powershell
   .\.\.scripts\powershell\powershell\test.ps1
   ```

3. **Build for ESP32:**
   ```powershell
   .\.\.scripts\powershell\powershell\build.ps1
   ```

4. **Build and upload:**
   ```powershell
   .\.\.scripts\powershell\powershell\build.ps1 --upload --monitor
   ```

## Cross-Platform Support

These scripts work on:
- **Windows** - Supports chocolatey and winget package managers

The scripts automatically detect your operating system and use the appropriate package manager.