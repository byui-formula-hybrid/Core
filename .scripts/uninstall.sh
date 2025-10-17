#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

while [[ $# -gt 0 ]]; do
    case $1 in
        --ci|-c)
            # Uninstall everything
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

echo ""
print_header "Uninstall Script"
echo ""

uninstall_build_artifacts
uninstall_pio_artifacts
uninstall_python_virtual_environment

if [ "$CI_MODE" == false ]; then
  uninstall_vscode_extensions
  uninstall_vscode

  uninstall_python

  uninstall_git

  uninstall_package_manager
fi
