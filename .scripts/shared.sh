# Shared functions for ESP32 Formula Hybrid Core Library scripts
# This file should be sourced by other scripts, not executed directly

# HELPERS AND UTILITIES
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

detect_os() {
    case "$OSTYPE" in
        darwin*)  OS="macos" ;;
        linux*)   OS="linux" ;;
        msys*|cygwin*|mingw*) OS="windows" ;;
        *)        OS="unknown" ;;
    esac
    export OS
}

print_info() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_header() {
    echo -e "${CYAN}🏎️  $1${NC}"
    echo "$(printf '=%.0s' $(seq 1 ${#1}))"
    echo ""
}

command_exists() {
    command -v "$1" >/dev/null 2>&1
}

show_project_info() {
    print_header "ESP32 Formula Hybrid Core Library"
    print_info "Operating System: $OS"
    print_info "Package Manager: $PACKAGE_MANAGER"
    print_info "Python: $PYTHON_CMD"
    print_info "Git: $(command_exists git && echo "✓" || echo "✗")"
    start_python_virtual_environment
    print_info "Python Virtual Environment: $( [ -n "$VIRTUAL_ENV" ] && echo "✓" || echo "✗")"
    print_info "PlatformIO: $(command_exists pio && echo "✓" || echo "✗")"
    stop_python_virtual_environment
}

# PACKAGE MANAGEMENT
install_package_manager() {
    case $OS in
        macos)
            if command_exists brew; then
                print_success "Homebrew already installed"
                return 0
            fi
            print_info "Installing Homebrew..."
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
            if [ $? -eq 0 ]; then
                print_success "Homebrew installed successfully"
            else
                print_error "Failed to install Homebrew"
                return 1
            fi
            ;;
        windows)
            if command_exists choco; then
                print_success "Chocolatey already installed"
                return 0
            fi
            print_info "Installing Chocolatey..."
            # this line is causing a syntax error not sure how to fix it...
            Set-ExecutionPolicy Bypass -Scope Process -Force; "[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))"
            # Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
            if [ $? -eq 0 ]; then
                print_success "Chocolatey installed successfully"
            else
                print_error "Failed to install Chocolatey"
                return 1
            fi
            ;;
        linux)
            if command_exists apt-get || command_exists yum || command_exists dnf || command_exists pacman; then
                print_success "Package manager already installed"
                return 0
            fi
            print_warning "No supported package manager found. Please install one manually (apt, yum, dnf, pacman)"
            return 1
            ;;
        *)
            print_warning "Unsupported OS for automatic package manager installation. Please install one manually."
            return 1
            ;;  
    esac
}

set_package_manager() {
    case $OS in
        macos)
            if command_exists brew; then
                PACKAGE_MANAGER="brew"
            else
                PACKAGE_MANAGER="none"
            fi
            ;;
        linux)
            if command_exists apt-get; then
                PACKAGE_MANAGER="apt"
            elif command_exists yum; then
                PACKAGE_MANAGER="yum"
            elif command_exists dnf; then
                PACKAGE_MANAGER="dnf"
            elif command_exists pacman; then
                PACKAGE_MANAGER="pacman"
            else
                PACKAGE_MANAGER="none"
            fi
            ;;
        windows)
            if command_exists choco; then
                PACKAGE_MANAGER="choco"
            elif command_exists winget; then
                PACKAGE_MANAGER="winget"
            else
                PACKAGE_MANAGER="none"
            fi
            ;;
        *)
            PACKAGE_MANAGER="none"
            ;;
    esac
    export PACKAGE_MANAGER
    print_info "Using package manager: $PACKAGE_MANAGER"
}

update_package_manager() {
    # Ask user if they want to update
    read -p "Do you want to update the package manager? (y/N): " UPDATE_PM
    if [[ "$UPDATE_PM" != "y" ]]; then
        print_warning "Skipping package manager update"
        return 1
    fi

    case $PACKAGE_MANAGER in
        brew)
            print_info "Updating Homebrew..."
            brew update
            ;;
        apt)
            print_info "Updating apt..."
            sudo apt-get update
            ;;
        yum)
            print_info "Updating yum..."
            sudo yum check-update
            ;;
        dnf)
            print_info "Updating dnf..."
            sudo dnf check-update
            ;;
        pacman)
            print_info "Updating pacman..."
            sudo pacman -Sy
            ;;
        choco)
            print_info "Updating Chocolatey..."
            choco upgrade all -y
            ;;
        winget)
            print_info "Updating winget packages..."
            winget upgrade --all --silent
            ;;
        *)
            print_warning "No supported package manager found for updating"
            return 1
            ;;
    esac
}

uninstall_package_manager() {
    # Ask user if they want to uninstall
    read -p "Do you want to uninstall the package manager? (y/N): " UNINSTALL_PM
    if [[ "$UNINSTALL_PM" != "y" ]]; then
        print_warning "Skipping package manager uninstallation"
        return 1
    fi

    case $PACKAGE_MANAGER in
        brew)
            print_info "Uninstalling Homebrew..."
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/uninstall.sh)"
            ;;
        choco)
            print_info "Uninstalling Chocolatey..."
            choco uninstall chocolatey -y
            ;;
        *)
            print_warning "Automatic uninstallation not supported for this package manager. Please uninstall it manually."
            return 1
            ;;
    esac
}

# PYTHON
install_python() {
    if command_exists python3; then
        print_success "Python 3 already installed"
        return 0
    elif command_exists python; then
        # Check if it's Python 3
        if python -c 'import sys; exit(0) if sys.version_info.major == 3 else exit(1)'; then
            print_success "Python 3 already installed"
            return 0
        fi
    fi
    
    print_info "Installing Python 3..."
    
    case $OS in
        macos)
            if [ "$PACKAGE_MANAGER" = "brew" ]; then
                brew install python3
            else
                print_warning "Please install Python 3 manually from https://python.org"
                return 1
            fi
            ;;
        linux)
            case $PACKAGE_MANAGER in
                apt)
                    sudo apt-get update && sudo apt-get install -y python3 python3-pip
                    ;;
                yum)
                    sudo yum install -y python3 python3-pip
                    ;;
                dnf)
                    sudo dnf install -y python3 python3-pip
                    ;;
                pacman)
                    sudo pacman -S python python-pip
                    ;;
                *)
                    print_warning "Please install Python 3 manually using your package manager"
                    return 1
                    ;;
            esac
            ;;
        windows)
            case $PACKAGE_MANAGER in
                choco)
                    choco install python3
                    ;;
                winget)
                    winget install Python.Python.3
                    ;;
                *)
                    print_warning "Please install Python 3 manually from https://python.org"
                    return 1
                    ;;
            esac
            ;;
    esac
}

set_python() {
    if command_exists python3; then
        PYTHON_CMD="python3"
    elif command_exists python; then
        PYTHON_CMD="python"
    else
        print_error "Python not found! Please install Python first"
        return 1
    fi
    export PYTHON_CMD
}

start_python_virtual_environment() {
    if [ -z "$PYTHON_CMD" ]; then
        set_python || return 1
    fi
    
    if [ -d ".venv" ]; then
        # Activate existing virtual environment
        source .venv/bin/activate
        print_info "Activated existing Python virtual environment"
    else
        # Create and activate new virtual environment
        $PYTHON_CMD -m venv .venv
        if [ $? -ne 0 ]; then
            print_error "Failed to create Python virtual environment"
            return 1
        fi
        source .venv/bin/activate
        print_info "Created and activated new Python virtual environment"
    fi
}

stop_python_virtual_environment() {
    if [ -n "$VIRTUAL_ENV" ]; then
        deactivate
        print_info "Deactivated Python virtual environment"
    fi
}

install_python_packages() {
    if [ -z "$PYTHON_CMD" ]; then
        set_python || return 1
    fi
    
    if [ ! -f "requirements.txt" ]; then
        print_warning "requirements.txt not found, skipping Python packages installation"
        return 1
    fi
    
    print_info "Installing Python packages from requirements.txt..."
    $PYTHON_CMD -m pip install --upgrade pip
    $PYTHON_CMD -m pip install -r requirements.txt
    if [ $? -eq 0 ]; then
        print_success "Python packages installed successfully"
    else
        print_error "Failed to install some Python packages"
        return 1
    fi
}

uninstall_python_virtual_environment() {
    if [ -d ".venv" ]; then
        rm -rf .venv
        print_success "Python virtual environment removed"
    else
        print_warning "No Python virtual environment found to remove"
    fi
}

uninstall_python() {
    # Ask user if they want to uninstall
    read -p "Do you want to uninstall Python? (y/N): " UNINSTALL_PYTHON
    if [[ "$UNINSTALL_PYTHON" != "y" ]]; then
        print_warning "Skipping Python uninstallation"
        return 1
    fi

    case $OS in
        macos)
            if [ "$PACKAGE_MANAGER" = "brew" ]; then
                brew uninstall python3
            else
                print_warning "Please uninstall Python 3 manually from https://python.org"
                return 1
            fi
            ;;
        linux)
            case $PACKAGE_MANAGER in
                apt)
                    sudo apt-get remove -y python3 python3-pip
                    ;;
                yum)
                    sudo yum remove -y python3 python3-pip
                    ;;
                dnf)
                    sudo dnf remove -y python3 python3-pip
                    ;;
                pacman)
                    sudo pacman -R python python-pip
                    ;;
                *)
                    print_warning "Please uninstall Python 3 manually using your package manager"
                    return 1
                    ;;
            esac
            ;;
        windows)
            case $PACKAGE_MANAGER in
                choco)
                    choco uninstall python3 -y
                    ;;
                winget)
                    winget uninstall Python.Python.3
                    ;;
                *)
                    print_warning "Please uninstall Python 3 manually from https://python.org"
                    return 1
                    ;;
            esac
            ;;
    esac
}

# GIT
install_git() {
    if command_exists git; then
        print_success "Git already installed"
        return 0
    fi
    
    print_info "Installing Git..."
    
    case $OS in
        macos)
            if [ "$PACKAGE_MANAGER" = "brew" ]; then
                brew install git
            else
                print_warning "Please install Git manually from https://git-scm.com/download/mac"
                return 1
            fi
            ;;
        linux)
            case $PACKAGE_MANAGER in
                apt)
                    sudo apt-get update && sudo apt-get install -y git
                    ;;
                yum)
                    sudo yum install -y git
                    ;;
                dnf)
                    sudo dnf install -y git
                    ;;
                pacman)
                    sudo pacman -S git
                    ;;
                *)
                    print_warning "Please install Git manually using your package manager"
                    return 1
                    ;;
            esac
            ;;
        windows)
            case $PACKAGE_MANAGER in
                choco)
                    choco install git
                    ;;
                winget)
                    winget install Git.Git
                    ;;
                *)
                    print_warning "Please install Git manually from https://git-scm.com/download/win"
                    return 1
                    ;;
            esac
            ;;
    esac
}

uninstall_git() {
    # Ask user if they want to uninstall
    read -p "Do you want to uninstall Git? (y/N): " UNINSTALL_GIT
    if [[ "$UNINSTALL_GIT" != "y" ]]; then
        print_warning "Skipping Git uninstallation"
        return 1
    fi

    case $OS in
        macos)
            if [ "$PACKAGE_MANAGER" = "brew" ]; then
                brew uninstall git
            else
                print_warning "Please uninstall Git manually from https://git-scm.com/download/mac"
                return 1
            fi
            ;;
        linux)
            case $PACKAGE_MANAGER in
                apt)
                    sudo apt-get remove -y git
                    ;;
                yum)
                    sudo yum remove -y git
                    ;;
                dnf)
                    sudo dnf remove -y git
                    ;;
                pacman)
                    sudo pacman -R git
                    ;;
                *)
                    print_warning "Please uninstall Git manually using your package manager"
                    return 1
                    ;;
            esac
            ;;
        windows)
            case $PACKAGE_MANAGER in
                choco)
                    choco uninstall git -y
                    ;;
                winget)
                    winget uninstall Git.Git
                    ;;
                *)
                    print_warning "Please uninstall Git manually from https://git-scm.com/download/win"
                    return 1
                    ;;
            esac
            ;;
    esac
}

# PLATFORMIO
install_platformio() {
    if command_exists pio; then
        print_success "PlatformIO already installed"
        return 0
    fi
    
    if [ -z "$PYTHON_CMD" ]; then
        set_python || return 1
    fi
    
    print_info "Installing PlatformIO Core..."
    $PYTHON_CMD -m pip install --upgrade platformio
    if [ $? -eq 0 ]; then
        print_success "PlatformIO Core installed successfully"
        return 0
    else
        print_error "Failed to install PlatformIO Core"
        return 1
    fi
}

install_platformio_dependencies() {
    if [ -z "$PYTHON_CMD" ]; then
        set_python || return 1
    fi
    
    print_info "Installing PlatformIO Core..."
    install_platformio
    if [ $? -eq 0 ]; then
        print_success "PlatformIO Core installed successfully"
    else
        print_error "Failed to install PlatformIO Core"
        return 1
    fi
}

uninstall_pio_artifacts() {
    if [ -d ".pio" ]; then
        rm -rf .pio
        print_success "PlatformIO artifacts removed"
    else
        print_warning "No PlatformIO artifacts found to remove"
    fi
}

# VS CODE
install_vs_code() {
    if command_exists code; then
        print_success "VS Code already installed"
        return 0
    elif command_exists code.cmd; then
        print_success "VS Code already installed"
        return 0
    fi

    # Ask user if they want to install vs code
    read -p "Do you want to install VS Code? (y/N): " INSTALL_VS_CODE
    if [[ "$INSTALL_VS_CODE" != "y" ]]; then
        print_warning "Skipping VS Code installation"
        return 1
    fi
    
    print_info "Installing VS Code..."
    
    case $OS in
        macos)
            if [ "$PACKAGE_MANAGER" = "brew" ]; then
                brew install --cask visual-studio-code
            else
                print_warning "Please install VS Code manually from https://code.visualstudio.com/download"
                return 1
            fi
            ;;
        linux)
            if [ "$PACKAGE_MANAGER" = "apt" ]; then
                wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.gpg
                sudo install -o root -g root -m 644 microsoft.gpg /usr/share/keyrings/
                sudo sh -c 'echo "deb [arch=amd64 signed-by=/usr/share/keyrings/microsoft.gpg] https://packages.microsoft.com/repos/vscode stable main" > /etc/apt/sources.list.d/vscode.list'
                sudo apt-get update
                sudo apt-get install -y code
                rm microsoft.gpg
            elif [ "$PACKAGE_MANAGER" = "yum" ] || [ "$PACKAGE_MANAGER" = "dnf" ]; then
                sudo rpm --import https://packages.microsoft.com/keys/microsoft.asc
                sudo sh -c 'echo -e "[code]\nname=Visual Studio Code\nbaseurl=https://packages.microsoft.com/yumrepos/vscode\nenabled=1\ngpgcheck=1\ngpgkey=https://packages.microsoft.com/keys/microsoft.asc" > /etc/yum.repos.d/vscode.repo'
                if [ "$PACKAGE_MANAGER" = "yum" ]; then
                    sudo yum check-update
                    sudo yum install -y code
                else
                    sudo dnf check-update
                    sudo dnf install -y code
                fi
            elif [ "$PACKAGE_MANAGER" = "pacman" ]; then
                print_warning "Please install VS Code manually from https://code.visualstudio.com/download or via AUR"
                return 1
            else
                print_warning "Please install VS Code manually from https://code.visualstudio.com/download"
                return 1
            fi
            ;;
        windows)
            if [ "$PACKAGE_MANAGER" = "choco" ]; then
                choco install vscode -y
            elif [ "$PACKAGE_MANAGER" = "winget" ]; then
                winget install --id Microsoft.VisualStudioCode -e --silent
            else
                print_warning "Please install VS Code manually from https://code.visualstudio.com/download"
                return 1
            fi
            ;;
        *)
            print_warning "Unsupported OS for automatic VS Code installation. Please install it manually."
            return 1
            ;;  
    esac
}

set_vscode() {
    if command_exists code; then
        VSCODE_CMD="code"
    elif command_exists code.cmd; then
        VSCODE_CMD="code.cmd"
    else
        print_warning "VS Code not found! Please install VS Code first"
        return 1
    fi

    export VSCODE_CMD
    print_info "VS Code command: $VSCODE_CMD"
}

uninstall_vscode() {
    # Ask user if they want to uninstall
    read -p "Do you want to uninstall VS Code? (y/N): " UNINSTALL_VS_CODE
    if [[ "$UNINSTALL_VS_CODE" != "y" ]]; then
        print_warning "Skipping VS Code uninstallation"
        return 1
    fi

    case $OS in
        macos)
            if [ "$PACKAGE_MANAGER" = "brew" ]; then
                brew uninstall --cask visual-studio-code
            else
                print_warning "Please uninstall VS Code manually from https://code.visualstudio.com/download"
                return 1
            fi
            ;;
        linux)
            if [ "$PACKAGE_MANAGER" = "apt" ]; then
                sudo apt-get remove -y code
            elif [ "$PACKAGE_MANAGER" = "yum" ] || [ "$PACKAGE_MANAGER" = "dnf" ]; then
                if [ "$PACKAGE_MANAGER" = "yum" ]; then
                    sudo yum remove -y code
                else
                    sudo dnf remove -y code
                fi
            elif [ "$PACKAGE_MANAGER" = "pacman" ]; then
                print_warning "Please uninstall VS Code manually from https://code.visualstudio.com/download or via AUR"
                return 1
            else
                print_warning "Please uninstall VS Code manually from https://code.visualstudio.com/download"
                return 1
            fi
            ;;
        windows)
            if [ "$PACKAGE_MANAGER" = "choco" ]; then
                choco uninstall vscode -y
            elif [ "$PACKAGE_MANAGER" = "winget" ]; then
                winget uninstall Microsoft.VisualStudioCode
            else
                print_warning "Please uninstall VS Code manually from https://code.visualstudio.com/download"
                return 1
            fi
            ;;
        *)
            print_warning "Unsupported OS for automatic VS Code uninstallation. Please uninstall it manually."
            return 1
            ;;  
    esac
}

install_vscode_extensions() {
    # Check to see if the vscode command is available
    if [ -z "$VSCODE_CMD" ]; then
        set_vscode || return 1
    fi

    # Ask user if they want to install extensions
    read -p "Do you want to install recommended VS Code extensions? (y/N): " INSTALL_EXTENSIONS
    if [[ "$INSTALL_EXTENSIONS" != "y" ]]; then
        print_warning "Skipping VS Code extensions installation"
        return 1
    fi

    install_vscode_extension "platformio.platformio-ide" "PlatformIO IDE"
    install_vscode_extension "bierner.markdown-mermaid" "Markdown Mermaid Preview"
    install_vscode_extension "yzhang.markdown-all-in-one" "Markdown All in One"
    install_vscode_extension "ms-python.python" "Python"
    install_vscode_extension "GitHub.copilot" "Copilot"
    install_vscode_extension "ms-vscode.cpptools-extension-pack" "C/C++ Extension Pack"
    install_vscode_extension "redhat.vscode-yaml" "YAML Language Support"
    install_vscode_extension "eamodio.gitlens" "GitLens"
    install_vscode_extension "hbenl.vscode-test-explorer" "Test Explorer UI"
    install_vscode_extension "oderwat.indent-rainbow" "Indent Rainbow"
    install_vscode_extension "yzhang.markdown-all-in-one" "Markdown All in One"
    install_vscode_extension "aaron-bond.better-comments" "Better Comments"
    install_vscode_extension "yzhang.markdown-all-in-one" "Markdown All in One"
}

uninstall_vscode_extensions() {
    # Check to see if the vscode command is available
    if [ -z "$VSCODE_CMD" ]; then
        set_vscode || return 1
    fi

    # Ask user if they want to uninstall extensions
    read -p "Do you want to uninstall recommended VS Code extensions? (y/N): " UNINSTALL_EXTENSIONS
    if [[ "$UNINSTALL_EXTENSIONS" != "y" ]]; then
        print_warning "Skipping VS Code extensions uninstallation"
        return 1
    fi

    uninstall_vscode_extension "platformio.platformio-ide" "PlatformIO IDE"
    uninstall_vscode_extension "bierner.markdown-mermaid" "Markdown Mermaid Preview"
    uninstall_vscode_extension "yzhang.markdown-all-in-one" "Markdown All in One"
    uninstall_vscode_extension "ms-python.python" "Python"
    uninstall_vscode_extension "GitHub.copilot" "Copilot"
    uninstall_vscode_extension "ms-vscode.cpptools-extension-pack" "C/C++ Extension Pack"
    uninstall_vscode_extension "redhat.vscode-yaml" "YAML Language Support"
    uninstall_vscode_extension "eamodio.gitlens" "GitLens"
    uninstall_vscode_extension "hbenl.vscode-test-explorer" "Test Explorer UI"
    uninstall_vscode_extension "oderwat.indent-rainbow" "Indent Rainbow"
    uninstall_vscode_extension "aaron-bond.better-comments" "Better Comments"
    uninstall_vscode_extension "yzhang.markdown-all-in-one" "Markdown All in One"
}

install_vscode_extension() {
    local extension_id="$1"
    local extension_name="$2"

    $VSCODE_CMD --install-extension "$extension_id" --force
}

uninstall_vscode_extension() {
    local extension_id="$1"
    local extension_name="$2"
    
    $VSCODE_CMD --uninstall-extension "$extension_id"
}

# TESTS
count_tests() {
    find test -name "test_main.cpp" 2>/dev/null | wc -l | tr -d ' '
}

list_test_modules() {
    find test -name "test_main.cpp" 2>/dev/null | sed 's|test/test_||' | sed 's|/test_main.cpp||' | sort
}

# BUILD
uninstall_build_artifacts() {
    if [ -d "build" ]; then
        rm -rf build
        print_success "Build artifacts removed"
    else
        print_warning "No build artifacts found to remove"
    fi
}