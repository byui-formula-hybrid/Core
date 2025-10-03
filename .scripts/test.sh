#!/bin/bash

# Script to build CoreLib and run all unit tests
# Usage: ./run_unit_tests.sh

# Exit immediately if a command exits with a non-zero status
set -e

# Activate Python virtual environment
if [ -d ".venv" ]; then
    source .venv/bin/activate
else
    echo ".venv not found. Please create the virtual environment first." 
    exit 1
fi

# Install Conan dependencies
echo "Installing Conan dependencies..."
conan install . --output-folder=build/Release --build=missing

# Create build directory if it doesn't exist
mkdir -p build/Release
cd build/Release

# Generate build system with CMake
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake

# Build library and tests
cmake --build .

# Run all tests
ctest --output-on-failure

# Deactivate virtual environment
deactivate