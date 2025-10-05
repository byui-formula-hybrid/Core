#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

# Initialize
init_script

print_header "Uninstall Script"

print_warning "This will remove PlatformIO and project dependencies."
print_warning "VS Code, Python, Git, and package managers will NOT be removed."
echo ""

# Cross-platform read input
if [ "$OS" = "windows" ]; then
    echo -n "Are you sure you want to continue? (y/N): "
    read response
else
    read -p "Are you sure you want to continue? (y/N): " -n 1 -r response
    echo ""
fi

if [[ ! $response =~ ^[Yy]$ ]]; then
    print_info "Uninstall cancelled."
    exit 0
fi

echo ""
print_info "Removing PlatformIO and project files..."

# Remove PlatformIO
if command_exists pio; then
    print_info "Removing PlatformIO Core..."
    
    case $OS in
        macos|linux)
            # Remove PlatformIO directory
            rm -rf "$HOME/.platformio"
            
            # Remove from shell config
            shell_config=$(get_shell_config)
            if [ -f "$shell_config" ]; then
                # Remove platformio-related lines
                case $OS in
                    macos)
                        sed -i '' '/platformio\|\.local\/bin/d' "$shell_config"
                        ;;
                    linux)
                        sed -i '/platformio\|\.local\/bin/d' "$shell_config"
                        ;;
                esac
            fi
            
            # Remove pip installation
            if command_exists python3; then
                python3 -m pip uninstall -y platformio
            elif command_exists python; then
                python -m pip uninstall -y platformio
            fi
            ;;
        windows)
            # Remove PlatformIO directory
            if [ -d "$USERPROFILE/.platformio" ]; then
                rm -rf "$USERPROFILE/.platformio"
            fi
            
            # Remove pip installation
            if command_exists python; then
                python -m pip uninstall -y platformio
            fi
            
            print_warning "Please manually remove PlatformIO from your PATH if needed"
            ;;
    esac
    
    print_success "PlatformIO removed"
else
    print_success "PlatformIO not found (already removed)"
fi

# Clean project build files
if [ -d ".pio" ]; then
    print_info "Removing project build files..."
    rm -rf .pio
    print_success "Build files removed"
fi

# Clean VS Code settings (optional)
echo ""
if [ "$OS" = "windows" ]; then
    echo -n "Remove VS Code workspace settings? (y/N): "
    read vscode_response
else
    read -p "Remove VS Code workspace settings? (y/N): " -n 1 -r vscode_response
    echo ""
fi

if [[ $vscode_response =~ ^[Yy]$ ]]; then
    if [ -d ".vscode" ]; then
        rm -rf .vscode
        print_success "VS Code settings removed"
    fi
fi

echo ""
print_success "Uninstall complete!"
echo ""
print_info "What was NOT removed:"
echo "- VS Code application"
echo "- Python"
echo "- Git"
echo "- Package managers (brew, apt, etc.)"
echo ""
print_info "To completely clean your system:"
case $OS in
    macos)
        echo "- Remove VS Code: rm -rf '/Applications/Visual Studio Code.app'"
        echo "- Remove Homebrew: /bin/bash -c \"\$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/uninstall.sh)\""
        ;;
    linux)
        echo "- Remove VS Code: sudo apt remove code (or equivalent for your distro)"
        echo "- Remove Python: sudo apt remove python3 (or equivalent)"
        ;;
    windows)
        echo "- Remove VS Code: Use Windows 'Add or Remove Programs'"
        echo "- Remove Python: Use Windows 'Add or Remove Programs'"
        ;;
esac
echo ""