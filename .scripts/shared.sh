# Shared functions for ESP32 Formula Hybrid Core Library scripts
# This file should be sourced by other scripts, not executed directly

# Colors for output (works on most terminals)
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Detect operating system
detect_os() {
    case "$OSTYPE" in
        darwin*)  OS="macos" ;;
        linux*)   OS="linux" ;;
        msys*|cygwin*|mingw*) OS="windows" ;;
        *)        OS="unknown" ;;
    esac
    export OS
}

# Print colored output
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

# Check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Check if PlatformIO is installed
check_platformio() {
    if ! command_exists pio; then
        print_error "PlatformIO not found! Please run the install script first"
        return 1
    fi
    return 0
}

# Get package manager for the OS
get_package_manager() {
    case $OS in
        macos)
            if command_exists brew; then
                echo "brew"
            else
                echo "none"
            fi
            ;;
        linux)
            if command_exists apt-get; then
                echo "apt"
            elif command_exists yum; then
                echo "yum"
            elif command_exists dnf; then
                echo "dnf"
            elif command_exists pacman; then
                echo "pacman"
            else
                echo "none"
            fi
            ;;
        windows)
            if command_exists choco; then
                echo "choco"
            elif command_exists winget; then
                echo "winget"
            else
                echo "none"
            fi
            ;;
        *)
            echo "none"
            ;;
    esac
}

# Install Python 3
install_python() {
    local pkg_manager=$(get_package_manager)
    
    if command_exists python3; then
        print_success "Python 3 already installed"
        return 0
    fi
    
    print_info "Installing Python 3..."
    
    case $OS in
        macos)
            if [ "$pkg_manager" = "brew" ]; then
                brew install python3
            else
                print_warning "Please install Python 3 manually from https://python.org"
                return 1
            fi
            ;;
        linux)
            case $pkg_manager in
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
            case $pkg_manager in
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

# Install Git
install_git() {
    local pkg_manager=$(get_package_manager)
    
    if command_exists git; then
        print_success "Git already installed"
        return 0
    fi
    
    print_info "Installing Git..."
    
    case $OS in
        macos)
            if [ "$pkg_manager" = "brew" ]; then
                brew install git
            else
                print_warning "Please install Git manually from https://git-scm.com"
                return 1
            fi
            ;;
        linux)
            case $pkg_manager in
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
            case $pkg_manager in
                choco)
                    choco install git
                    ;;
                winget)
                    winget install Git.Git
                    ;;
                *)
                    print_warning "Please install Git manually from https://git-scm.com"
                    return 1
                    ;;
            esac
            ;;
    esac
}

# Install PlatformIO
install_platformio() {
    if command_exists pio; then
        print_success "PlatformIO already installed"
        return 0
    fi
    
    print_info "Installing PlatformIO Core..."
    
    # Use pip to install PlatformIO (works on all platforms)
    if command_exists python3; then
        python3 -m pip install --user platformio
    elif command_exists python; then
        python -m pip install --user platformio
    else
        print_error "Python not found! Please install Python first"
        return 1
    fi
    
    # Add to PATH (platform-specific)
    case $OS in
        macos|linux)
            # Add to shell profile
            local shell_profile=""
            if [ -n "$ZSH_VERSION" ]; then
                shell_profile="$HOME/.zshrc"
            elif [ -n "$BASH_VERSION" ]; then
                shell_profile="$HOME/.bashrc"
            else
                shell_profile="$HOME/.profile"
            fi
            
            if [ -f "$shell_profile" ]; then
                echo 'export PATH="$HOME/.local/bin:$PATH"' >> "$shell_profile"
                export PATH="$HOME/.local/bin:$PATH"
            fi
            ;;
        windows)
            print_warning "Please add %USERPROFILE%\\.platformio\\penv\\Scripts to your PATH"
            ;;
    esac
}

# Get shell configuration file
get_shell_config() {
    if [ -n "$ZSH_VERSION" ]; then
        echo "$HOME/.zshrc"
    elif [ -n "$BASH_VERSION" ]; then
        echo "$HOME/.bashrc"
    else
        echo "$HOME/.profile"
    fi
}

# Count test files
count_tests() {
    find test -name "test_main.cpp" 2>/dev/null | wc -l | tr -d ' '
}

# List test modules
list_test_modules() {
    find test -name "test_main.cpp" 2>/dev/null | sed 's|test/test_||' | sed 's|/test_main.cpp||' | sort
}

# Show project info
show_project_info() {
    print_header "ESP32 Formula Hybrid Core Library"
    print_info "Operating System: $OS"
    print_info "Package Manager: $(get_package_manager)"
    print_info "Python: $(command_exists python3 && echo "✓" || echo "✗")"
    print_info "Git: $(command_exists git && echo "✓" || echo "✗")"
    print_info "PlatformIO: $(command_exists pio && echo "✓" || echo "✗")"
    echo ""
}

# Initialize - call this at the start of scripts
init_script() {
    detect_os
    show_project_info
}