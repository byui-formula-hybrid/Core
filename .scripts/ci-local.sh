#!/bin/bash

# Local CI Simulation Script
# Runs the same validations as the GitHub Actions pipeline

set -e  # Exit on any error

echo "🏎️  Local CI Pipeline Simulation"
echo "================================="
echo ""

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

# Check if we're in the right directory
if [ ! -f "platformio.ini" ]; then
    print_error "platformio.ini not found. Please run this script from the project root."
    exit 1
fi

print_status "Starting local CI pipeline simulation..."
echo ""

# Step 1: Native Tests
print_status "Step 1/2: Running Native Tests"
echo "-----------------------------------"

if pio test -e native; then
    print_success "Native tests passed!"
    TESTS_PASSED=true
else
    print_error "Native tests failed!"
    TESTS_PASSED=false
fi

echo ""

# Step 2: ESP32 Build
print_status "Step 2/2: Building ESP32 Firmware"
echo "------------------------------------"

if pio run -e esp32dev; then
    print_success "ESP32 build successful!"
    BUILD_PASSED=true
    
    # Check build artifacts
    if [ -f ".pio/build/esp32dev/firmware.bin" ]; then
        FIRMWARE_SIZE=$(stat -f%z .pio/build/esp32dev/firmware.bin 2>/dev/null || stat -c%s .pio/build/esp32dev/firmware.bin 2>/dev/null)
        print_success "Firmware binary created (${FIRMWARE_SIZE} bytes)"
    fi
else
    print_error "ESP32 build failed!"
    BUILD_PASSED=false
fi

echo ""

# Final Results
print_status "Pipeline Results Summary"
echo "============================"

if $TESTS_PASSED; then
    print_success "Native Tests: PASSED"
else
    print_error "Native Tests: FAILED"
fi

if $BUILD_PASSED; then
    print_success "ESP32 Build: PASSED"
else
    print_error "ESP32 Build: FAILED"
fi

echo ""

# Overall result
if $TESTS_PASSED && $BUILD_PASSED; then
    print_success "🎉 All validations passed! Ready for CI pipeline."
    echo ""
    print_status "Your code is ready to push to GitHub! 🚀"
    exit 0
else
    print_error "💥 Some validations failed!"
    echo ""
    print_warning "Fix the issues above before pushing to GitHub."
    exit 1
fi