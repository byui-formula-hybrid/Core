# Core

## Overview

**Core** is a modular C++ library that organizes your business logic and core functionality into separate modules. Each module is self-contained and can be built individually or all together.

Modules included in Core:

* `core` — base logic used by other modules
* `battery` — battery management
* `can` — Controller Area Network interface
* `display` — dashboard/display logic
* `imd` — insulation monitoring device
* `inverter` — inverter control
* `pedals` — pedal input handling
* `pump` — pump control

---

## Why Modular?

Breaking the library into modules allows you to:

* Work on each part independently.
* Include only the parts you need in other projects.
* Easily add dependencies between modules in the future.
* Run unit tests locally to verify correctness.

---

## Folder Structure

```
Root/
├── CMakeLists.txt           # Main project file
├── conanfile.py             # Conan dependency and packaging info
└── modules/
    ├── core/
    │   ├── include/core/file.h
    │   └── src/file.cpp
    ├── battery/
    │   ├── include/core/battery/file.h
    │   └── src/file.cpp
    ├── can/
    │   ├── include/core/can/file.h
    │   └── src/file.cpp
    ├── display/
    │   ├── include/core/display/file.h
    │   └── src/file.cpp
    ├── imd/
    │   ├── include/core/imd/file.h
    │   └── src/file.cpp
    ├── inverter/
    │   ├── include/core/inverter/file.h
    │   └── src/file.cpp
    ├── pedals/
    │   ├── include/core/pedals/file.h
    │   └── src/file.cpp
    └── pump/
        ├── include/core/pump/file.h
        └── src/file.cpp
```

* `include/` — headers that describe how to use the module
* `src/` — source files that implement the module functionality

---

## Setup: Required Software

Before building and testing Core, make sure the following software is installed on your system.

### Linux (Ubuntu/Debian)

**Install essential build tools and dependencies:**
```bash
# Update package manager
sudo apt update

# Install build essentials, CMake, GCC, and Python
sudo apt install build-essential cmake g++ python3 python3-venv python3-pip

# Verify installations
gcc --version
cmake --version
python3 --version
```

**For other Linux distributions:**
- **CentOS/RHEL/Fedora**: Replace `apt` with `yum` or `dnf`
- **Arch Linux**: Use `pacman -S base-devel cmake gcc python python-pip`

### macOS

**Prerequisites:**
- macOS 10.15 (Catalina) or later
- Xcode Command Line Tools

**Install Xcode Command Line Tools:**
```bash
xcode-select --install
```

**Install Homebrew (if not already installed):**
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

**Install required packages:**
```bash
# Install CMake and Python
brew install cmake python@3.11

# Verify installations
clang --version
cmake --version
python3 --version
```

**Note:** macOS uses Apple Clang by default, which is fully compatible with this project.

### Windows

**Option 1: Visual Studio (Recommended)**

1. **Install Visual Studio Community** (free):
   - Download from [Visual Studio](https://visualstudio.microsoft.com/vs/community/)
   - During installation, select "Desktop development with C++" workload
   - This includes MSVC compiler and Windows SDK

2. **Install CMake**:
   - Download from [CMake Downloads](https://cmake.org/download/)
   - Choose "Windows x64 Installer"
   - During installation, select "Add CMake to system PATH"

3. **Install Python**:
   - Download from [Python.org](https://www.python.org/downloads/)
   - **Important:** Check "Add Python to PATH" during installation
   - Choose "Install for all users" if you have admin rights

**Option 2: MinGW-w64 (Alternative)**

1. **Install MSYS2**:
   - Download from [MSYS2.org](https://www.msys2.org/)
   - Follow installation instructions

2. **Install build tools via MSYS2**:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-python
   ```

**Verify Windows Installation:**
```cmd
# Open Command Prompt and verify:
cl          # Should show MSVC compiler info (Option 1)
gcc --version   # Should show GCC version (Option 2)
cmake --version
python --version
```

### Verification Steps (All Platforms)

After installation, verify everything is working:

```bash
# Check compiler
gcc --version     # Linux/macOS/Windows(MinGW)
clang --version   # macOS
cl                # Windows (MSVC)

# Check CMake
cmake --version

# Check Python
python3 --version  # Linux/macOS
python --version   # Windows

# Check Python can create virtual environments
python3 -m venv test_env && rm -rf test_env  # Linux/macOS
python -m venv test_env && rmdir /s test_env # Windows
```

### Minimum Version Requirements

- **C++ Compiler**: 
  - GCC 9+ or Clang 10+ (Linux/macOS)
  - MSVC 2019+ or MinGW-w64 9+ (Windows)
- **CMake**: 3.20 or later
- **Python**: 3.8 or later
- **Operating System**:
  - Linux: Ubuntu 20.04+, CentOS 8+, or equivalent
  - macOS: 10.15 (Catalina) or later
  - Windows: Windows 10 or later

> **Note:** Python packages (including Conan) will be installed in a virtual environment in the next section, not system-wide.

---

## Python Setup Using Virtual Environment (venv)

To ensure consistent Python versions and isolate dependencies, use a virtual environment.

### 1. Create a Virtual Environment

```bash
# Linux/macOS
python3 -m venv .venv

# Windows
python -m venv .venv
```

### 2. Activate the Environment

```bash
# Linux/macOS
source .venv/bin/activate

# Windows
.venv\Scripts\activate
```

### 3. Install Required Python Tools Inside venv

```bash
pip install --upgrade pip
pip install conan
```

### 4. Using the Virtual Environment

* Activate the venv whenever building or running tests.
* All Python commands (e.g., `conan install`) will use the isolated environment.
* Deactivate with:

```bash
deactivate
```

*Advantages:* consistent Python version, no system conflicts, easy to recreate on other machines.

---

## Building the Library

The Core library provides a flexible build script that allows you to build the entire project or specific modules in different configurations.

### Build Script Usage

```bash
# Basic usage - build everything in Release mode
./.scripts/build.sh

# Build in Debug mode
./.scripts/build.sh --debug

# Clean build (removes build directory first)
./.scripts/build.sh --clean

# Build only a specific module
./.scripts/build.sh --module=can

# Verbose build output
./.scripts/build.sh --verbose

# Get help
./.scripts/build.sh --help
```

### Build Script Options

| Option | Description |
|--------|-------------|
| `--debug` | Build in Debug mode (default: Release) |
| `--release` | Build in Release mode (explicit) |
| `--clean` | Clean build directory before building |
| `--module=NAME` | Build only specific module (core, battery, can, display, imd, inverter, pedals, pump) |
| `--verbose`, `-v` | Enable verbose build output |
| `--help`, `-h` | Show help message |

### Common Build Scenarios

```bash
# Development workflow - quick build of module you're working on
./.scripts/build.sh --module=can --debug

# Clean rebuild for release
./.scripts/build.sh --clean --release

# Build everything with verbose output for debugging
./.scripts/build.sh --verbose --debug

# Quick check that everything compiles
./.scripts/build.sh
```

### Build Output

The build script creates:
- **Static libraries**: `libcorelib_<module>.a` (Linux/macOS) or `corelib_<module>.lib` (Windows)
- **Test executables**: `test_<module>`
- **Build artifacts** in: `build/Release/` or `build/Debug/`

### When to Use Build vs Test Scripts

- **Use `./.scripts/build.sh`** when:
  - You want to quickly check compilation without running tests
  - You're developing and need fast iteration cycles
  - You want to build specific modules only
  - You need different build configurations (Debug/Release)

- **Use `./.scripts/test.sh`** when:
  - You want to build AND run all tests
  - You're doing final verification before commits
  - You want the complete development workflow

---

## Build and Run Unit Tests Locally

### Prerequisites

1. **Activate the virtual environment** (required for all builds):
   ```bash
   # Linux/macOS
   source .venv/bin/activate
   
   # Windows
   .venv\Scripts\activate
   ```

2. **First-time Conan setup** (only needed once):
   ```bash
   conan profile detect
   ```

### Using the Test Script (Recommended)

The easiest way to build and test is using the provided script:

```bash
# Linux/macOS
./.scripts/test.sh
```

This script will:
- Check for and activate the virtual environment
- Install Conan dependencies (including GTest for testing)
- Configure CMake with the Conan toolchain
- Build all modules and tests
- Run all unit tests with CTest

> **Note:** If you only want to build without running tests, use `./.scripts/build.sh` instead (see Building the Library section above).

### Manual Build (Alternative)

If you prefer to run commands manually:

#### Linux/macOS
```bash
# Activate venv first
source .venv/bin/activate

# Install dependencies
conan install . --output-folder=build/Release --build=missing

# Create and enter build directory
mkdir -p build/Release
cd build/Release

# Configure with CMake
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake

# Build library and tests
cmake --build .

# Run tests
ctest --output-on-failure
```

#### Windows (PowerShell)
```powershell
# Activate venv
.venv\Scripts\Activate.ps1

# Install dependencies
conan install . --output-folder=build/Release --build=missing

# Create and enter build directory
mkdir -p build/Release; cd build/Release

# Configure with CMake
cmake ..\.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake

# Build library and tests
cmake --build .

# Run tests
ctest --output-on-failure
```

* This will compile the library and run all tests defined in the `tests/` folder.
* Tests use Google Test framework, which is automatically managed by Conan.
* The build process creates static libraries for each module with the `corelib_` prefix (e.g., `corelib_battery`).

### Adding Dependencies Between Modules

```cmake
# Example: IMD module depends on battery module
target_link_libraries(corelib_imd PUBLIC corelib_battery)
```

* In Conan, you would also define `requires` for the component in `package_info()`.

### Adding New Files

* Keep headers in `include/` updated.
* Build system will pick up new `.cpp` files automatically (if using `file(GLOB ...)`) or you can list them manually in the `CMakeLists.txt` for each module.

---

## Notes

* This library is source-only; no prebuilt binaries are required.
* All modules are designed to be tested locally using Conan + CMake + CTest.
* Module names in CMake targets use lowercase with `corelib_` prefix (e.g., `corelib_battery`).
