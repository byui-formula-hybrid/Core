#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

echo ""
print_header "Uninstall Script"
echo ""

uninstall_build_artifacts
uninstall_pio_artifacts

uninstall_vscode_extensions
uninstall_vscode

uninstall_python_virtual_environment
uninstall_python

uninstall_git

uninstall_package_manager
