#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export SCRIPT_DIR

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

CI_MODE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --ci|-c)
            # Setup for CI environment
            CI_MODE=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  --ci, -c               Setup for CI environment"
            echo "  -h, --help             Show this help message"
            echo ""
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

detect_os

echo ""
print_header "Installation Script"
echo ""

install_package_manager
set_package_manager
update_package_manager

install_git

install_python
set_python
start_python_virtual_environment
install_python_packages
stop_python_virtual_environment

if [ "$CI_MODE" == false ]; then
  install_vs_code
  set_vscode
  install_vscode_extensions
fi

echo ""
show_project_info

echo ""
print_success "Installation complete!"
echo ""