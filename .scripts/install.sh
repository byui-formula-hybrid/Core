#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export SCRIPT_DIR

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

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
install_platformio_dependencies
stop_python_virtual_environment

install_vs_code
set_vscode
install_vscode_extensions

echo ""
show_project_info

echo ""
print_success "Installation complete!"
echo ""