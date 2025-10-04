# Core Project Copilot Instructions

## Architecture Overview
Core is a modular C++ library for a Formula Hybrid vehicle control system. Each module follows a strict structure:
- `modules/{name}/include/core/{name}/` - headers with module interface
- `modules/{name}/src/` - implementation files  
- `tests/{name}/` - Google Test unit tests for the module

Modules: `core` (base), `battery`, `can`, `display`, `imd`, `inverter`, `pedals`, `pump`

## Build System Patterns
- **CMake 3.16+** with C++17 standard
- **Conan 2.0** for dependency management (GTest pinned to 1.14.0)
- Each module creates library target `corelib_{MODULE_NAME}`
- Tests link against `corelib_{MODULE_NAME}` and `GTest::gtest`

### Key Build Commands
```bash
# Full setup (includes deps, venv, conan profile)
./.scripts/setup.sh

# Build all modules
conan install . --output-folder=build --build=missing
cmake --preset conan-release
cmake --build --preset conan-release

# Run tests
ctest --preset conan-release
```

## Development Workflows
1. **New module creation**: Copy existing module structure, update `MODULE_NAME` in CMakeLists.txt
2. **Adding dependencies**: Update `target_link_libraries` in module's CMakeLists.txt
3. **Version pinning**: Update `requirements.txt` for Python deps, `conanfile.py` for C++ deps

## Project-Specific Conventions
- Headers use `core/{module_name}/` namespace structure
- Test files named `test_*.cpp` with automatic discovery via `file(GLOB)`
- Environment setup uses pinned versions (Python 3.11, CMake 3.24+, specific Conan versions)
- CAN bus communication follows automotive patterns (see `docs/Design.md`)

## Critical File Locations
- `conanfile.py` - Dependency versions and package configuration
- `requirements.txt` - Python package versions (Conan 2.0.17)
- `.scripts/setup.sh` - Automated environment setup with version validation
- `docs/Design.md` - System architecture and CAN message protocols
- `CMakeUserPresets.json` - Build preset configuration

## Dependencies & Integration
- **External**: GTest for testing, Conan for C++ package management
- **Internal**: `core` module provides base functionality for other modules
- **Communication**: CAN bus protocol for inter-ECU communication (pedals ↔ brain ↔ motor controllers)

## Safety-Critical Considerations
This is automotive firmware with ASIL-like safety requirements. Pay attention to:
- Dual redundant sensors (especially in pedals module)
- Heartbeat/watchdog patterns in communication
- Fail-safe behaviors and fault handling