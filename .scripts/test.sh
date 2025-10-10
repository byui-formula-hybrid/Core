#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

# Initialize
init_script

print_header "Test Suite"
# Activate .venv
if [ -d ".venv" ]; then
    source .venv/bin/activate
else
    print_error ".venv not found! Please run the install script first."
    exit 1
fi

# Check if PlatformIO is installed
if ! check_platformio; then
    exit 1
fi

# Function to run tests
run_tests() {
    print_info "Running native unit tests..."
    echo ""
    
    # Run tests with verbose output if requested
    local cmd="python -m platformio test -e native"
    if [ "$VERBOSE" = true ]; then
        cmd="$cmd -v"
    fi
    
    if $cmd; then
        echo ""
        print_success "All tests passed!"
        return 0
    else
        echo ""
        print_error "Some tests failed!"
        return 1
    fi
}

# Function to show test summary
show_summary() {
    echo ""
    print_info "Test Summary"
    echo "==============="
    
    # Count test files
    local test_count=$(count_tests)
    print_info "Test suites found: $test_count"
    
    # List test modules
    print_info "Test modules:"
    list_test_modules | while read module; do
        echo "  • $module"
    done
}

# Parse command line arguments
VERBOSE=false
FILTER=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -f|--filter)
            FILTER="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  -v, --verbose    Show detailed test output"
            echo "  -f, --filter     Run specific test module (e.g., test_core_module)"
            echo "  -h, --help       Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                          # Run all tests"
            echo "  $0 --verbose                # Run all tests with verbose output"
            echo "  $0 --filter test_core_module # Run only core module tests"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Show project info
print_info "Environment: native (unit testing only)"
echo ""

# Show test summary
show_summary

echo ""

# Run specific test if filter is provided
if [ ! -z "$FILTER" ]; then
    print_info "Running filtered tests: $FILTER"
    echo ""
    cmd="python -m platformio test -e native --filter $FILTER"
    if [ "$VERBOSE" = true ]; then
        cmd="$cmd -v"
    fi
    
    if $cmd; then
        echo ""
        print_success "Filtered tests passed!"
    else
        echo ""
        print_error "Filtered tests failed!"
        exit 1
    fi
else
    # Run all tests
    run_tests
    TEST_RESULT=$?
fi

echo ""
print_info "Quick commands:"
echo "  Run all tests:     .scripts/test.sh"
echo "  Verbose output:    .scripts/test.sh --verbose"
echo "  Specific module:   .scripts/test.sh --filter test_core_module"
echo "  Build project:     .scripts/build.sh"
echo "  Clean build:       pio run -t clean"
deactivate

# Exit with test result
if [ ! -z "$FILTER" ]; then
    exit 0
else
    exit $TEST_RESULT
fi