#!/bin/bash

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source shared functions
source "$SCRIPT_DIR/shared.sh"

# Initialize
init_script

print_header "Installation Script"
# Ensure Python is installed
install_python

# Create .venv if missing
if [ ! -d ".venv" ]; then
    print_info "Creating Python virtual environment (.venv)..."
    python3 -m venv .venv
fi

# Activate .venv
if [ -d ".venv" ]; then
    print_info "Activating .venv..."
    source .venv/bin/activate
else
    print_error ".venv not found! Aborting."
    exit 1
fi

# Upgrade pip and install PlatformIO in .venv
print_info "Upgrading pip and installing PlatformIO in .venv..."
pip install --upgrade pip
pip install platformio

# Install other dependencies in .venv as needed
# (Add pip install commands here if needed)

# Deactivate .venv after setup
deactivate

# Install package manager if needed (macOS only)
if [ "$OS" = "macos" ] && [ "$(get_package_manager)" = "none" ]; then
    print_info "Installing Homebrew (macOS package manager)..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# Update package manager
pkg_manager=$(get_package_manager)
if [ "$pkg_manager" != "none" ]; then
    print_info "Updating package manager..."
    case $pkg_manager in
        brew)
            brew update
            ;;
        apt)
            sudo apt-get update
            ;;
        yum)
            sudo yum update -y
            ;;
        dnf)
            sudo dnf update -y
            ;;
        *)
            print_info "Skipping package manager update"
            ;;
    esac
fi

# Install dependencies
install_python
install_git
install_platformio

# Install VS Code (optional)
print_info "Installing VS Code (optional)..."
case $OS in
    macos)
        if [ -d "/Applications/Visual Studio Code.app" ]; then
            print_success "VS Code already installed"
        elif [ "$pkg_manager" = "brew" ]; then
            brew install --cask visual-studio-code
        else
            print_warning "Please install VS Code manually from https://code.visualstudio.com"
        fi
        ;;
    linux)
        case $pkg_manager in
            apt)
                wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
                sudo install -o root -g root -m 644 packages.microsoft.gpg /etc/apt/trusted.gpg.d/
                sudo sh -c 'echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/trusted.gpg.d/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" > /etc/apt/sources.list.d/vscode.list'
                sudo apt-get update && sudo apt-get install -y code
                ;;
            yum|dnf)
                sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc
                sudo sh -c 'echo -e "[code]\nname=Visual Studio Code\nbaseurl=https://packages.microsoft.com/yumrepos/vscode\nenabled=1\ngpgcheck=1\ngpgkey=https://packages.microsoft.com/keys/microsoft.asc" > /etc/yum.repos.d/vscode.repo'
                $pkg_manager install -y code
                ;;
            *)
                print_warning "Please install VS Code manually from https://code.visualstudio.com"
                ;;
        esac
        ;;
    windows)
        case $pkg_manager in
            choco)
                choco install vscode
                ;;
            winget)
                winget install Microsoft.VisualStudioCode
                ;;
            *)
                print_warning "Please install VS Code manually from https://code.visualstudio.com"
                ;;
        esac
        ;;
esac

# Install project dependencies
print_info "Installing project dependencies..."
if [ -f "platformio.ini" ]; then
    pio pkg install
    print_success "Project dependencies installed"
else
    print_warning "No platformio.ini found. Run this script from the project root."
fi

# Install VS Code extensions
install_recommended_extensions

echo ""
print_success "Installation complete!"
echo ""
print_info "Next steps:"
echo "1. Restart your terminal (or source your shell config)"
echo "2. Run '.scripts/test.sh' to verify installation"
echo "3. Run '.scripts/build.sh' to build the project"
echo ""
print_info "Installed VS Code extensions:"
echo "- PlatformIO IDE (platformio.platformio-ide)"
echo "- C/C++ Extension Pack (ms-vscode.cpptools-extension-pack)"
echo "- Wokwi Simulator (wokwi.wokwi-vscode)"
echo "- Markdown Mermaid Preview (bierner.markdown-mermaid)"
echo "- YAML Language Support (redhat.vscode-yaml)"
echo ""
print_info "Quick commands:"
echo "- Test:  .scripts/test.sh"
echo "- Build: .scripts/build.sh"
echo "- Clean: pio run -t clean"
echo ""