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
    
    # Detect if we're in a virtual environment
    if [ -n "$VIRTUAL_ENV" ] || [ -n "$CONDA_DEFAULT_ENV" ]; then
        print_info "Virtual environment detected, removing from current environment..."
        # Remove from virtual environment
        if command_exists python3; then
            python3 -m pip uninstall -y platformio
        elif command_exists python; then
            python -m pip uninstall -y platformio
        fi
    else
        # Remove from system installation
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
    fi
    
    print_success "PlatformIO removed"
else
    print_success "PlatformIO not found (already removed)"
fi

# Clean project build files and generated directories
print_info "Removing project build files and generated directories..."

# PlatformIO build artifacts
[ -d ".pio" ] && rm -rf .pio

# Python virtual environment and cache
[ -d ".venv" ] && rm -rf .venv
[ -d "__pycache__" ] && rm -rf __pycache__
find . -name "*.pyc" -delete 2>/dev/null
find . -name "*.pyo" -delete 2>/dev/null
find . -name "__pycache__" -type d -exec rm -rf {} + 2>/dev/null

# Build directories
find . -name "build" -type d -exec rm -rf {} + 2>/dev/null
find . -name "dist" -type d -exec rm -rf {} + 2>/dev/null

# Node.js dependencies (if any)
[ -d "node_modules" ] && rm -rf node_modules
[ -f "package-lock.json" ] && rm -f package-lock.json
[ -d ".npm" ] && rm -rf .npm

# Compiler outputs
find . -name "*.o" -delete 2>/dev/null
find . -name "*.obj" -delete 2>/dev/null
find . -name "*.elf" -delete 2>/dev/null
find . -name "*.bin" -delete 2>/dev/null
find . -name "*.hex" -delete 2>/dev/null
find . -name "*.map" -delete 2>/dev/null

# Documentation build
[ -d "docs/_build" ] && rm -rf docs/_build
[ -d "docs/build" ] && rm -rf docs/build

# Logs and debug files
find . -name "*.log" -delete 2>/dev/null
find . -name "*.out" -delete 2>/dev/null
[ -d "debug" ] && rm -rf debug
[ -d "logs" ] && rm -rf logs

# Test results and coverage
[ -d "test_results" ] && rm -rf test_results
[ -d "coverage" ] && rm -rf coverage
find . -name "*.gcov" -delete 2>/dev/null
find . -name "*.gcda" -delete 2>/dev/null
find . -name "*.gcno" -delete 2>/dev/null

# Temporary and cache files
[ -d "tmp" ] && rm -rf tmp
[ -d "temp" ] && rm -rf temp
[ -d ".cache" ] && rm -rf .cache
find . -name "*.tmp" -delete 2>/dev/null
find . -name "*.cache" -delete 2>/dev/null

# Hardware simulation files
find . -name "*.vcd" -delete 2>/dev/null
find . -name "*.fst" -delete 2>/dev/null
[ -d "simulation" ] && rm -rf simulation

# Race car specific directories
[ -d "telemetry_data" ] && rm -rf telemetry_data
[ -d "calibration_data" ] && rm -rf calibration_data
[ -d "race_logs" ] && rm -rf race_logs

# Backup files
find . -name "*.bak" -delete 2>/dev/null
find . -name "*.backup" -delete 2>/dev/null
find . -name "*_backup.*" -delete 2>/dev/null

# System files
find . -name ".DS_Store" -delete 2>/dev/null
find . -name "Thumbs.db" -delete 2>/dev/null
find . -name "Desktop.ini" -delete 2>/dev/null

print_success "Build files and generated directories removed"

# Uninstall VS Code extensions
echo ""
if [ "$OS" = "windows" ]; then
    echo -n "Remove project-specific VS Code extensions? (y/N): "
    read extension_response
else
    read -p "Remove project-specific VS Code extensions? (y/N): " -n 1 -r extension_response
    echo ""
fi

if [[ $extension_response =~ ^[Yy]$ ]]; then
    uninstall_project_extensions
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
echo "- Non-project VS Code extensions"
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