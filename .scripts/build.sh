#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

# Initialize
init_script

print_header "Build Script"

# Check if PlatformIO is installed
if ! check_platformio; then
    exit 1
fi

# Default values
ENVIRONMENT="esp32dev"
CLEAN=false
UPLOAD=false
MONITOR=false

# Function to show help
show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -e, --env ENVIRONMENT    Build environment (esp32dev, wokwi)"
    echo "                          Note: 'native' is for testing only, use .scripts/test.sh"
    echo "  -c, --clean              Clean build files before building"
    echo "  -u, --upload             Upload to device after building"
    echo "  -m, --monitor            Start serial monitor after upload"
    echo "  -h, --help               Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                       # Build for ESP32"
    echo "  $0 --clean               # Clean build for ESP32"
    echo "  $0 --upload              # Build and upload to ESP32"
    echo "  $0 --upload --monitor    # Build, upload, and monitor"
    echo "  $0 --env wokwi           # Build for Wokwi simulation"
    echo ""
    echo "For testing:"
    echo "  .scripts/test.sh         # Run unit tests (native environment)"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -e|--env)
            ENVIRONMENT="$2"
            shift 2
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -u|--upload)
            UPLOAD=true
            shift
            ;;
        -m|--monitor)
            MONITOR=true
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Validate environment
case $ENVIRONMENT in
    esp32dev|wokwi)
        ;;
    native)
        print_warning "Native environment is for testing only"
        print_info "Use '.scripts/test.sh' to run tests"
        print_info "Use '--env esp32dev' or '--env wokwi' for building"
        exit 1
        ;;
    *)
        print_error "Invalid environment: $ENVIRONMENT"
        echo "Valid environments: esp32dev, wokwi"
        echo "For testing, use: .scripts/test.sh"
        exit 1
        ;;
esac

# Show build info
print_info "Environment: $ENVIRONMENT"
print_info "Clean build: $([ "$CLEAN" = true ] && echo "Yes" || echo "No")"
print_info "Upload: $([ "$UPLOAD" = true ] && echo "Yes" || echo "No")"
print_info "Monitor: $([ "$MONITOR" = true ] && echo "Yes" || echo "No")"
echo ""

# Clean if requested
if [ "$CLEAN" = true ]; then
    print_info "Cleaning build files..."
    if pio run -e "$ENVIRONMENT" -t clean; then
        print_success "Clean completed"
    else
        print_error "Clean failed"
        exit 1
    fi
    echo ""
fi

# Build
print_info "Building for $ENVIRONMENT..."
if pio run -e "$ENVIRONMENT"; then
    echo ""
    print_success "Build successful!"
else
    echo ""
    print_error "Build failed!"
    exit 1
fi

# Upload if requested (only for ESP32)
if [ "$UPLOAD" = true ]; then
    if [ "$ENVIRONMENT" = "esp32dev" ]; then
        echo ""
        print_info "Uploading to ESP32..."
        if pio run -e "$ENVIRONMENT" -t upload; then
            print_success "Upload successful!"
        else
            print_error "Upload failed!"
            exit 1
        fi
    else
        print_warning "Upload only available for esp32dev environment"
    fi
fi

# Monitor if requested (only for ESP32)
if [ "$MONITOR" = true ]; then
    if [ "$ENVIRONMENT" = "esp32dev" ]; then
        echo ""
        print_info "Starting serial monitor..."
        print_warning "Press Ctrl+C to exit monitor"
        echo ""
        pio device monitor -e "$ENVIRONMENT"
    else
        print_warning "Monitor only available for esp32dev environment"
    fi
fi

echo ""
print_info "Quick commands:"
echo "  Build ESP32:       .scripts/build.sh"
echo "  Upload to ESP32:   .scripts/build.sh --upload"
echo "  Upload + Monitor:  .scripts/build.sh --upload --monitor"
echo "  Build for Wokwi:   .scripts/build.sh --env wokwi"
echo "  Clean build:       .scripts/build.sh --clean"
echo "  Run tests:         .scripts/test.sh"

# Special instructions for Wokwi
if [ "$ENVIRONMENT" = "wokwi" ]; then
    echo ""
    print_info "Wokwi Simulation Instructions:"
    echo "1. Open diagram.json in VS Code"
    echo "2. Press F1 → 'Wokwi: Start Simulator'"
    echo "3. Interact with the simulated ESP32"
fi