#!/bin/bash

# Core Project Setup Script
# Automatically detects platform and installs required dependencies
# Usage: ./setup.sh [--auto]

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Parse command line arguments
AUTO_INSTALL=false
while [[ $# -gt 0 ]]; do
    case $1 in
        --auto)
            AUTO_INSTALL=true
            shift
            ;;
        --help|-h)
            echo "Core Project Setup Script"
            echo ""
            echo "Usage: $0 [options]"
            echo ""
            echo "Options:"
            echo "  --auto               Automatically install missing dependencies without prompting"
            echo "  --help, -h           Show this help message"
            echo ""
            echo "This script will:"
            echo "  1. Detect your operating system"
            echo "  2. Check for required dependencies (CMake, Python, compilers)"
            echo "  3. Offer to install missing dependencies using system package managers"
            echo "  4. Set up Python virtual environment with Conan"
            echo "  5. Run initial Conan profile detection"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

echo -e "${BLUE}============================================${NC}"
echo -e "${BLUE}        Core Project Setup Script${NC}"
echo -e "${BLUE}============================================${NC}"
echo ""

# Function to print status messages
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to ask for user confirmation
ask_permission() {
    if [ "$AUTO_INSTALL" = true ]; then
        return 0
    fi
    
    local message="$1"
    echo -e "${YELLOW}$message (y/N):${NC} "
    read -r response
    case "$response" in
        [yY][eE][sS]|[yY])
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

# Detect operating system
detect_os() {
    print_status "Detecting operating system..."
    
    if [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        print_success "Detected: macOS"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
        if command -v apt-get &> /dev/null; then
            DISTRO="debian"
            print_success "Detected: Linux (Debian/Ubuntu)"
        elif command -v yum &> /dev/null || command -v dnf &> /dev/null; then
            DISTRO="rhel"
            print_success "Detected: Linux (RHEL/CentOS/Fedora)"
        elif command -v pacman &> /dev/null; then
            DISTRO="arch"
            print_success "Detected: Linux (Arch)"
        else
            DISTRO="unknown"
            print_warning "Detected: Linux (Unknown distribution)"
        fi
    elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        OS="windows"
        print_success "Detected: Windows"
    else
        OS="unknown"
        print_warning "Unknown operating system: $OSTYPE"
    fi
}

# Check if a command exists
command_exists() {
    command -v "$1" &> /dev/null
}

# Check CMake
check_cmake() {
    print_status "Checking for CMake..."
    
    if command_exists cmake; then
        CMAKE_VERSION=$(cmake --version | head -n1 | sed 's/cmake version //')
        print_success "CMake found: $CMAKE_VERSION"
        
        # Check minimum version (3.20)
        CMAKE_MAJOR=$(echo $CMAKE_VERSION | cut -d. -f1)
        CMAKE_MINOR=$(echo $CMAKE_VERSION | cut -d. -f2)
        
        if [ "$CMAKE_MAJOR" -gt 3 ] || ([ "$CMAKE_MAJOR" -eq 3 ] && [ "$CMAKE_MINOR" -ge 20 ]); then
            print_success "CMake version is sufficient (>= 3.20)"
            return 0
        else
            print_warning "CMake version $CMAKE_VERSION is too old (need >= 3.20)"
            return 1
        fi
    else
        print_warning "CMake not found"
        return 1
    fi
}

# Check Python
check_python() {
    print_status "Checking for Python..."
    
    # Try python3 first, then python
    for python_cmd in python3 python; do
        if command_exists $python_cmd; then
            PYTHON_VERSION=$($python_cmd --version 2>&1 | sed 's/Python //')
            PYTHON_MAJOR=$(echo $PYTHON_VERSION | cut -d. -f1)
            PYTHON_MINOR=$(echo $PYTHON_VERSION | cut -d. -f2)
            
            if [ "$PYTHON_MAJOR" -eq 3 ] && [ "$PYTHON_MINOR" -ge 8 ]; then
                print_success "Python found: $PYTHON_VERSION ($python_cmd)"
                PYTHON_CMD=$python_cmd
                return 0
            else
                print_warning "Python $PYTHON_VERSION is too old (need >= 3.8)"
            fi
        fi
    done
    
    print_warning "Suitable Python (>= 3.8) not found"
    return 1
}

# Check C++ compiler
check_compiler() {
    print_status "Checking for C++ compiler..."
    
    if [[ "$OS" == "macos" ]]; then
        if command_exists clang++; then
            CLANG_VERSION=$(clang++ --version | head -n1)
            print_success "Clang++ found: $CLANG_VERSION"
            return 0
        elif command_exists g++; then
            GCC_VERSION=$(g++ --version | head -n1)
            print_success "G++ found: $GCC_VERSION"
            return 0
        fi
    elif [[ "$OS" == "linux" ]]; then
        if command_exists g++; then
            GCC_VERSION=$(g++ --version | head -n1)
            print_success "G++ found: $GCC_VERSION"
            return 0
        elif command_exists clang++; then
            CLANG_VERSION=$(clang++ --version | head -n1)
            print_success "Clang++ found: $CLANG_VERSION"
            return 0
        fi
    elif [[ "$OS" == "windows" ]]; then
        if command_exists cl; then
            print_success "MSVC found"
            return 0
        elif command_exists g++; then
            GCC_VERSION=$(g++ --version | head -n1)
            print_success "G++ found: $GCC_VERSION"
            return 0
        fi
    fi
    
    print_warning "No suitable C++ compiler found"
    return 1
}

# Install dependencies based on OS
install_dependencies() {
    print_status "Installing missing dependencies..."
    
    if [[ "$OS" == "macos" ]]; then
        install_macos_deps
    elif [[ "$OS" == "linux" ]]; then
        install_linux_deps
    elif [[ "$OS" == "windows" ]]; then
        install_windows_deps
    else
        print_error "Automatic installation not supported for this OS"
        print_error "Please install dependencies manually:"
        print_error "  - CMake (>= 3.20)"
        print_error "  - Python (>= 3.8)"
        print_error "  - C++ compiler (GCC 9+, Clang 10+, or MSVC 2019+)"
        exit 1
    fi
}

install_macos_deps() {
    # Check if Homebrew is installed
    if ! command_exists brew; then
        if ask_permission "Homebrew is required but not installed. Install Homebrew?"; then
            print_status "Installing Homebrew..."
            /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
        else
            print_error "Homebrew is required for automatic installation on macOS"
            exit 1
        fi
    fi
    
    # Check for Xcode Command Line Tools
    if ! xcode-select -p &> /dev/null; then
        if ask_permission "Xcode Command Line Tools are required. Install them?"; then
            print_status "Installing Xcode Command Line Tools..."
            xcode-select --install
            print_status "Please complete the Xcode Command Line Tools installation and run this script again"
            exit 0
        else
            print_error "Xcode Command Line Tools are required"
            exit 1
        fi
    fi
    
    # Install missing packages
    BREW_PACKAGES=""
    
    if ! check_cmake; then
        BREW_PACKAGES="$BREW_PACKAGES cmake"
    fi
    
    if ! check_python; then
        BREW_PACKAGES="$BREW_PACKAGES python@3.11"
    fi
    
    if [ -n "$BREW_PACKAGES" ]; then
        if ask_permission "Install missing packages via Homebrew:$BREW_PACKAGES?"; then
            print_status "Installing packages:$BREW_PACKAGES"
            brew install $BREW_PACKAGES
        fi
    fi
}

install_linux_deps() {
    if [[ "$DISTRO" == "debian" ]]; then
        # Ubuntu/Debian
        APT_PACKAGES=""
        
        if ! check_cmake; then
            APT_PACKAGES="$APT_PACKAGES cmake"
        fi
        
        if ! check_python; then
            APT_PACKAGES="$APT_PACKAGES python3 python3-venv python3-pip"
        fi
        
        if ! check_compiler; then
            APT_PACKAGES="$APT_PACKAGES build-essential g++"
        fi
        
        if [ -n "$APT_PACKAGES" ]; then
            if ask_permission "Install missing packages via apt:$APT_PACKAGES?"; then
                print_status "Updating package lists..."
                sudo apt update
                print_status "Installing packages:$APT_PACKAGES"
                sudo apt install -y $APT_PACKAGES
            fi
        fi
        
    elif [[ "$DISTRO" == "rhel" ]]; then
        # RHEL/CentOS/Fedora
        if command_exists dnf; then
            PKG_MGR="dnf"
        else
            PKG_MGR="yum"
        fi
        
        YUM_PACKAGES=""
        
        if ! check_cmake; then
            YUM_PACKAGES="$YUM_PACKAGES cmake"
        fi
        
        if ! check_python; then
            YUM_PACKAGES="$YUM_PACKAGES python3 python3-pip"
        fi
        
        if ! check_compiler; then
            YUM_PACKAGES="$YUM_PACKAGES gcc-c++ make"
        fi
        
        if [ -n "$YUM_PACKAGES" ]; then
            if ask_permission "Install missing packages via $PKG_MGR:$YUM_PACKAGES?"; then
                print_status "Installing packages:$YUM_PACKAGES"
                sudo $PKG_MGR install -y $YUM_PACKAGES
            fi
        fi
        
    elif [[ "$DISTRO" == "arch" ]]; then
        # Arch Linux
        PACMAN_PACKAGES=""
        
        if ! check_cmake; then
            PACMAN_PACKAGES="$PACMAN_PACKAGES cmake"
        fi
        
        if ! check_python; then
            PACMAN_PACKAGES="$PACMAN_PACKAGES python python-pip"
        fi
        
        if ! check_compiler; then
            PACMAN_PACKAGES="$PACMAN_PACKAGES base-devel gcc"
        fi
        
        if [ -n "$PACMAN_PACKAGES" ]; then
            if ask_permission "Install missing packages via pacman:$PACMAN_PACKAGES?"; then
                print_status "Installing packages:$PACMAN_PACKAGES"
                sudo pacman -S --noconfirm $PACMAN_PACKAGES
            fi
        fi
    else
        print_error "Automatic installation not supported for this Linux distribution"
        exit 1
    fi
}

install_windows_deps() {
    print_error "Automatic installation on Windows is not yet implemented"
    print_error "Please install the following manually:"
    print_error "  1. Visual Studio Community with C++ workload"
    print_error "  2. CMake (from cmake.org)"
    print_error "  3. Python (from python.org)"
    print_error "See README.md for detailed instructions"
    exit 1
}

# Setup Python virtual environment
setup_python_venv() {
    print_status "Setting up Python virtual environment..."
    
    if [ -d ".venv" ]; then
        print_success "Virtual environment already exists"
    else
        if ask_permission "Create Python virtual environment (.venv)?"; then
            print_status "Creating virtual environment..."
            $PYTHON_CMD -m venv .venv
            print_success "Virtual environment created"
        else
            print_warning "Skipping virtual environment setup"
            return 1
        fi
    fi
    
    # Activate virtual environment
    print_status "Activating virtual environment..."
    source .venv/bin/activate
    
    # Upgrade pip
    print_status "Upgrading pip..."
    pip install --upgrade pip
    
    # Install Conan
    print_status "Installing Conan..."
    pip install conan
    
    print_success "Python environment setup complete"
    return 0
}

# Setup Conan
setup_conan() {
    print_status "Setting up Conan..."
    
    # Check if Conan profile exists
    if conan profile show default &> /dev/null; then
        print_success "Conan profile already configured"
    else
        print_status "Detecting Conan profile..."
        if conan profile detect; then
            print_success "Conan profile created"
        else
            print_warning "Conan profile detection had issues, but may still work"
        fi
    fi
}

# Run verification build
verify_setup() {
    print_status "Verifying setup with test build..."
    
    if ask_permission "Run a test build to verify everything works?"; then
        print_status "Running test build..."
        if ./.scripts/test.sh; then
            print_success "Test build completed successfully!"
            print_success "Setup verification passed ✓"
        else
            print_error "Test build failed"
            print_error "Please check the error messages above"
            exit 1
        fi
    else
        print_warning "Skipping verification build"
        print_status "You can manually verify the setup by running: ./.scripts/test.sh"
    fi
}

# Main setup flow
main() {
    detect_os
    echo ""
    
    # Check all dependencies
    print_status "Checking system dependencies..."
    echo ""
    
    CMAKE_OK=false
    PYTHON_OK=false
    COMPILER_OK=false
    
    if check_cmake; then
        CMAKE_OK=true
    fi
    
    if check_python; then
        PYTHON_OK=true
    fi
    
    if check_compiler; then
        COMPILER_OK=true
    fi
    
    echo ""
    
    # Install missing dependencies
    if [ "$CMAKE_OK" = false ] || [ "$PYTHON_OK" = false ] || [ "$COMPILER_OK" = false ]; then
        print_warning "Some dependencies are missing"
        if ask_permission "Install missing dependencies automatically?"; then
            install_dependencies
            echo ""
            
            # Re-check after installation
            print_status "Re-checking dependencies after installation..."
            check_cmake && CMAKE_OK=true
            check_python && PYTHON_OK=true
            check_compiler && COMPILER_OK=true
            echo ""
        else
            print_error "Dependencies are required to continue"
            print_error "Please install them manually and run this script again"
            exit 1
        fi
    fi
    
    # Verify all dependencies are now available
    if [ "$CMAKE_OK" = false ] || [ "$PYTHON_OK" = false ] || [ "$COMPILER_OK" = false ]; then
        print_error "Some dependencies are still missing after installation"
        exit 1
    fi
    
    print_success "All system dependencies are available!"
    echo ""
    
    # Setup Python environment
    if setup_python_venv; then
        echo ""
        setup_conan
        echo ""
        verify_setup
    else
        print_warning "Python environment setup was skipped"
        print_warning "You may need to set up the virtual environment manually"
    fi
    
    echo ""
    print_success "Setup completed successfully!"
    echo ""
    echo -e "${GREEN}Next steps:${NC}"
    echo "  1. Activate the virtual environment: ${BLUE}source .venv/bin/activate${NC}"
    echo "  2. Build the project: ${BLUE}./.scripts/build.sh${NC}"
    echo "  3. Run tests: ${BLUE}./.scripts/test.sh${NC}"
    echo "  4. See README.md for more development information"
    echo ""
}

# Run main function
main "$@"