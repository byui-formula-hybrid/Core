#!/bin/bash

# Script to build CoreLib and run all unit tests
# Usage: ./test.sh [--debug|--release]

# Default build type
BUILD_TYPE="Release"

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
        --help|-h)
            echo "CoreLib Test Script"
            echo ""
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  --debug              Build and test in Debug mode"
            echo "  --release            Build and test in Release mode (default)"
            echo "  --help, -h           Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                   # Test in Release mode"
            echo "  $0 --debug           # Test in Debug mode"
            echo "  $0 --release         # Test in Release mode (explicit)"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Exit immediately if a command exits with a non-zero status
set -e

echo "CoreLib Test Script"
echo "=================="
echo "Build Type: $BUILD_TYPE"
echo ""

# Activate Python virtual environment
if [ -d ".venv" ]; then
    source .venv/bin/activate
else
    echo ".venv not found. Please create the virtual environment first." 
    exit 1
fi

# Install Conan dependencies
echo "Installing Conan dependencies for $BUILD_TYPE build..."
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

# Generate build system with CMake
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=build/$BUILD_TYPE/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# Build library and tests
cmake --build .

# Run all tests
ctest --output-on-failure

# Deactivate virtual environment
deactivate