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

## Recommended VS Code Extensions

To enhance your development experience with this C++ CMake project, we recommend installing the following VS Code extensions. These extensions provide better code navigation, debugging, and build integration.

### Essential Extensions

#### C/C++ Development
- **[C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)** (`ms-vscode.cpptools`)
  - IntelliSense, debugging, and code browsing for C/C++
  - **Required** for proper C++ language support

- **[C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)** (`ms-vscode.cpptools-extension-pack`)
  - Includes C/C++ tools, CMake Tools, and other useful extensions
  - **Recommended** as a convenient bundle

#### Build System Support
- **[CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)** (`ms-vscode.cmake-tools`)
  - CMake integration with build, debug, and project configuration
  - **Essential** for this CMake-based project

- **[CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)** (`twxs.cmake`)
  - CMake language support with syntax highlighting and completion
  - **Recommended** for editing CMakeLists.txt files

### Code Quality & Formatting

- **[clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)** (`llvm-vs-code-extensions.vscode-clangd`)
  - Advanced C++ language server with better IntelliSense than the default
  - **Optional but recommended** - disable C/C++ IntelliSense if using this

- **[C/C++ Clang Command Adapter](https://marketplace.visualstudio.com/items?itemName=mitaki28.vscode-clang)** (`mitaki28.vscode-clang`)
  - Clang-based code completion and diagnostics
  - **Alternative** to clangd extension

### Testing & Debugging

- **[Test Explorer UI](https://marketplace.visualstudio.com/items?itemName=hbenl.vscode-test-explorer)** (`hbenl.vscode-test-explorer`)
  - Unified test runner interface
  - **Recommended** for running Google Test suites

- **[C++ TestMate](https://marketplace.visualstudio.com/items?itemName=matepek.vscode-catch2-test-adapter)** (`matepek.vscode-catch2-test-adapter`)
  - Test adapter for Google Test and other C++ testing frameworks
  - **Recommended** for integrated test running

### Git & Version Control

- **[GitLens](https://marketplace.visualstudio.com/items?itemName=eamodio.gitlens)** (`eamodio.gitlens`)
  - Enhanced Git capabilities with blame annotations and history
  - **Recommended** for collaborative development

### Python Support (for Conan)

- **[Python](https://marketplace.visualstudio.com/items?itemName=ms-python.python)** (`ms-python.python`)
  - Python language support for Conan scripts and build automation
  - **Recommended** since this project uses Conan

### Additional Utilities

- **[Better Comments](https://marketplace.visualstudio.com/items?itemName=aaron-bond.better-comments)** (`aaron-bond.better-comments`)
  - Color-coded comment annotations (TODO, FIXME, etc.)
  - **Optional** but helpful for code documentation

- **[indent-rainbow](https://marketplace.visualstudio.com/items?itemName=oderwat.indent-rainbow)** (`oderwat.indent-rainbow`)
  - Color-coded indentation levels
  - **Optional** but helpful for readability

### Quick Installation

You can install all essential extensions at once by running these commands in VS Code's integrated terminal:

```bash
# Essential extensions
code --install-extension ms-vscode.cpptools-extension-pack
code --install-extension ms-vscode.cmake-tools
code --install-extension twxs.cmake

# Testing extensions
code --install-extension hbenl.vscode-test-explorer
code --install-extension matepek.vscode-catch2-test-adapter

# Python support
code --install-extension ms-python.python

# Git enhancement
code --install-extension eamodio.gitlens
```

### VS Code Workspace Settings

Consider adding these settings to your workspace configuration (`.vscode/settings.json`):

```json
{
    "cmake.buildDirectory": "${workspaceFolder}/build",
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "python.defaultInterpreterPath": "./.venv/bin/python",
    "files.associations": {
        "*.h": "cpp",
        "*.hpp": "cpp"
    }
}
```

### Extension Compatibility Notes

- If you choose to use **clangd**, disable the default C/C++ IntelliSense to avoid conflicts:
  ```json
  {
      "C_Cpp.intelliSenseEngine": "Disabled"
  }
  ```

- For **CMake Tools** to work properly with Conan, ensure your build directory matches the Conan output folder structure.

---

## CMake Presets Setup

CMake presets provide a standardized way to configure build settings across different IDEs and development environments. This project uses presets to integrate with Conan and provide consistent build configurations.

### Understanding CMake Presets in This Project

This project uses two types of preset files:

- **`CMakePresets.json`** - Generated by Conan, contains project-wide configurations
- **`CMakeUserPresets.json`** - User-specific overrides (ignored by git)

### Setting Up CMake Presets

#### 1. Generate Conan Presets (Required)

Before using presets, you must generate them with Conan:

```bash
# Activate virtual environment
source .venv/bin/activate  # Linux/macOS
# .venv\Scripts\activate   # Windows

# Generate presets for Release configuration
conan install . --output-folder=build/Release --build=missing -s build_type=Release

# Generate presets for Debug configuration (optional)
conan install . --output-folder=build/Debug --build=missing -s build_type=Debug
```

This creates `CMakePresets.json` files in the build directories that contain:
- Conan toolchain configuration
- Compiler settings
- Library paths
- Build type configurations

#### 2. Create User Presets (Optional)

Create a `CMakeUserPresets.json` file in the project root for personal customizations:

```json
{
    "version": 4,
    "vendor": {
        "conan": {}
    },
    "include": [
        "build/Release/generators/CMakePresets.json"
    ],
    "configurePresets": [
        {
            "name": "default-release",
            "inherits": "conan-release",
            "displayName": "Release Build",
            "description": "Release build with Conan dependencies"
        },
        {
            "name": "default-debug", 
            "inherits": "conan-debug",
            "displayName": "Debug Build",
            "description": "Debug build with Conan dependencies",
            "cacheVariables": {
                "CMAKE_BUILD_TYPE": "Debug"
            }
        }
    ],
    "buildPresets": [
        {
            "name": "build-release",
            "configurePreset": "default-release",
            "displayName": "Build Release"
        },
        {
            "name": "build-debug",
            "configurePreset": "default-debug", 
            "displayName": "Build Debug"
        }
    ],
    "testPresets": [
        {
            "name": "test-release",
            "configurePreset": "default-release",
            "displayName": "Test Release",
            "output": {
                "outputOnFailure": true
            }
        },
        {
            "name": "test-debug",
            "configurePreset": "default-debug",
            "displayName": "Test Debug",
            "output": {
                "outputOnFailure": true
            }
        }
    ]
}
```

### IDE Integration

#### VS Code with CMake Tools

1. **Install CMake Tools extension** (covered in VS Code Extensions section)

2. **Configure workspace settings** in `.vscode/settings.json`:
   ```json
   {
       "cmake.buildDirectory": "${workspaceFolder}/build",
       "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
       "cmake.generator": "Unix Makefiles",
       "cmake.preferredGenerators": ["Unix Makefiles", "Ninja"]
   }
   ```

3. **Select a preset**:
   - Open Command Palette (`Cmd+Shift+P` / `Ctrl+Shift+P`)
   - Run "CMake: Select Configure Preset"
   - Choose your desired preset (e.g., "default-release")

4. **Build and test**:
   - Use `Cmd+Shift+P` → "CMake: Build" 
   - Use `Cmd+Shift+P` → "CMake: Run Tests"

#### Visual Studio

1. **Open the project folder** in Visual Studio
2. **Presets are automatically detected** from `CMakeUserPresets.json`
3. **Select preset** from the configuration dropdown
4. **Build** using standard Visual Studio build commands

#### CLion

1. **Open project** in CLion
2. **Go to File → Settings → Build, Execution, Deployment → CMake**
3. **Select "Use CMake presets"**
4. **Choose your preset** from the dropdown

### Using Presets with Command Line

You can also use presets directly with CMake:

```bash
# Configure using a preset
cmake --preset default-release

# Build using a preset  
cmake --build --preset build-release

# Run tests using a preset
ctest --preset test-release
```

### Preset Best Practices

#### What to Include in User Presets
- Personal build directories
- IDE-specific configurations
- Custom compiler flags for development
- Personal debugging settings

#### What NOT to Include in User Presets
- Absolute paths (use variables like `${sourceDir}`)
- Team-wide settings (these belong in the main project configuration)
- Sensitive information

### Troubleshooting Presets

#### Common Issues

**"No presets found"**
- Ensure you've run `conan install` to generate the base presets
- Check that `CMakeUserPresets.json` includes the correct Conan preset paths

**"Preset inheritance failed"**
- Verify the included preset files exist in the specified paths
- Make sure the inherited preset names match exactly

**"Toolchain not found"**
- Confirm Conan generated the toolchain files
- Check that the toolchain path in the preset is correct

#### Regenerating Presets

If you encounter issues, regenerate the Conan presets:

```bash
# Clean and regenerate
rm -rf build/
source .venv/bin/activate
conan install . --output-folder=build/Release --build=missing -s build_type=Release
```

### Alternative: Using Build Scripts

If you prefer not to use presets, the provided build scripts (`./.scripts/build.sh` and `./.scripts/test.sh`) handle all configuration automatically and are often simpler for new contributors.

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
