#!/bin/bash

# Script to build CoreLib modules
# Usage: ./build.sh [options]

# Default values
BUILD_TYPE="Release"
CLEAN_BUILD=false
MODULE=""
VERBOSE=false
HELP=false

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --release)
            BUILD_TYPE="Release"
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --module=*)
            MODULE="${1#*=}"
            shift
            ;;
        --verbose|-v)
            VERBOSE=true
            shift
            ;;
        --help|-h)
            HELP=true
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Show help
if [ "$HELP" = true ]; then
    echo "CoreLib Build Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  --debug              Build in Debug mode (default: Release)"
    echo "  --release            Build in Release mode"
    echo "  --clean              Clean build directory before building"
    echo "  --module=MODULE      Build only specific module (e.g., --module=can)"
    echo "  --verbose, -v        Enable verbose build output"
    echo "  --help, -h           Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                   # Build everything in Release mode"
    echo "  $0 --debug           # Build everything in Debug mode"
    echo "  $0 --clean --debug   # Clean build and build in Debug mode"
    echo "  $0 --module=can      # Build only the CAN module"
    echo "  $0 --verbose         # Build with verbose output"
    echo ""
    echo "Available modules: core, battery, can, display, imd, inverter, pedals, pump"
    exit 0
fi

# Exit immediately if a command exits with a non-zero status
set -e

# Function to check for required tools
check_dependencies() {
    local missing_deps=()
    
    if ! command -v cmake &> /dev/null; then
        missing_deps+=("cmake")
    fi
    
    if ! command -v python3 &> /dev/null && ! command -v python &> /dev/null; then
        missing_deps+=("python")
    fi
    
    if [ ${#missing_deps[@]} -ne 0 ]; then
        echo "Error: Missing required dependencies: ${missing_deps[*]}"
        echo ""
        echo "To install missing dependencies automatically, run:"
        echo "  ./.scripts/setup.sh"
        echo ""
        echo "Or install manually:"
        for dep in "${missing_deps[@]}"; do
            echo "  - $dep"
        done
        echo ""
        echo "See README.md for detailed installation instructions"
        exit 1
    fi
}

# Convert BUILD_TYPE to lowercase for directory name
BUILD_DIR_NAME=$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')
BUILD_DIR="build/$BUILD_TYPE"

echo "CoreLib Build Script"
echo "==================="
echo "Build Type: $BUILD_TYPE"
echo "Build Directory: $BUILD_DIR"
if [ -n "$MODULE" ]; then
    echo "Target Module: $MODULE"
fi
echo ""

# Check for required dependencies
check_dependencies

# Activate Python virtual environment
if [ -d ".venv" ]; then
    echo "Activating virtual environment..."
    source .venv/bin/activate
else
    echo "Error: .venv not found. Please create the virtual environment first."
    echo "Run: python3 -m venv .venv && source .venv/bin/activate && pip install conan"
    exit 1
fi

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Install Conan dependencies
echo "Installing Conan dependencies..."
if [ "$BUILD_TYPE" = "Release" ]; then
    conan install . --output-folder=build/Release --build=missing -s build_type=Release
    BUILD_DIR="build/Release"
else
    conan install . --output-folder=build/Debug --build=missing -s build_type=Debug
    BUILD_DIR="build/Debug"
fi

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo "Configuring with CMake..."
CMAKE_ARGS="../.. -DCMAKE_TOOLCHAIN_FILE=build/$BUILD_TYPE/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [ "$VERBOSE" = true ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_VERBOSE_MAKEFILE=ON"
fi

cmake $CMAKE_ARGS

# Build the project
echo "Building..."
if [ -n "$MODULE" ]; then
    # Build specific module
    MODULE_TARGET="corelib_$MODULE"
    echo "Building module: $MODULE (target: $MODULE_TARGET)"
    
    if [ "$VERBOSE" = true ]; then
        cmake --build . --target "$MODULE_TARGET" --verbose
    else
        cmake --build . --target "$MODULE_TARGET"
    fi
    
    # Also build the corresponding test if it exists
    TEST_TARGET="test_$MODULE"
    echo "Building test for module: $MODULE (target: $TEST_TARGET)"
    
    if [ "$VERBOSE" = true ]; then
        cmake --build . --target "$TEST_TARGET" --verbose || echo "Note: No test target found for $MODULE"
    else
        cmake --build . --target "$TEST_TARGET" || echo "Note: No test target found for $MODULE"
    fi
else
    # Build everything
    echo "Building all modules..."
    
    if [ "$VERBOSE" = true ]; then
        cmake --build . --verbose
    else
        cmake --build .
    fi
fi

echo ""
echo "Build completed successfully!"
echo "Build artifacts are in: $BUILD_DIR"

# List the built libraries
echo ""
echo "Built libraries:"
if [ -n "$MODULE" ]; then
    find . -name "libcorelib_$MODULE.a" -o -name "corelib_$MODULE.lib" 2>/dev/null || echo "  No libraries found for module $MODULE"
else
    find . -name "libcorelib_*.a" -o -name "corelib_*.lib" 2>/dev/null || echo "  No libraries found"
fi

# List the built test executables
echo ""
echo "Built test executables:"
if [ -n "$MODULE" ]; then
    find . -name "test_$MODULE" -o -name "test_$MODULE.exe" 2>/dev/null || echo "  No test executable found for module $MODULE"
else
    find . -name "test_*" -type f -executable 2>/dev/null | grep -v "\.exe$" || echo "  No test executables found"
    find . -name "test_*.exe" 2>/dev/null || true
fi

# Deactivate virtual environment
deactivate