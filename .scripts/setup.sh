#!/bin/bash

# Core Project Setup Script
# Automatically detects platform and installs required dependencies
# Usage: ./setup.sh [--auto]

set -e

# =============================================================================
# VERSION CONFIGURATION
# =============================================================================
# Pin specific versions for consistency across development environments

# Required versions (minimum)
REQUIRED_CMAKE_VERSION="3.24.0"
REQUIRED_PYTHON_VERSION="3.11.0"

# Preferred/pinned versions for installation
PINNED_CMAKE_VERSION="3.28.1"
PINNED_PYTHON_VERSION="3.11"

# Compiler version requirements
MIN_GCC_VERSION="11"
MIN_CLANG_VERSION="14"
MIN_MSVC_VERSION="2022"

# =============================================================================
# VERSION COMPARISON UTILITIES
# =============================================================================

# Compare version strings (returns 0 if version1 >= version2)
version_compare() {
    local version1="$1"
    local version2="$2"
    
    # Remove any non-numeric prefixes and suffixes
    version1=$(echo "$version1" | sed 's/[^0-9.].*//')
    version2=$(echo "$version2" | sed 's/[^0-9.].*//')
    
    if [ "$version1" = "$version2" ]; then
        return 0
    fi
    
    local IFS=.
    local i ver1=($version1) ver2=($version2)
    
    # Fill empty fields with zeros
    for ((i=${#ver1[@]}; i<${#ver2[@]}; i++)); do
        ver1[i]=0
    done
    for ((i=${#ver2[@]}; i<${#ver1[@]}; i++)); do
        ver2[i]=0
    done
    
    # Compare version numbers
    for ((i=0; i<${#ver1[@]}; i++)); do
        if [[ ${ver1[i]} -gt ${ver2[i]} ]]; then
            return 0
        elif [[ ${ver1[i]} -lt ${ver2[i]} ]]; then
            return 1
        fi
    done
    
    return 0
}

# Extract version number from command output
extract_version() {
    local cmd_output="$1"
    local pattern="$2"
    
    if [ -n "$pattern" ]; then
        echo "$cmd_output" | sed -n "s/$pattern/\1/p" | head -n1
    else
        echo "$cmd_output" | grep -oE '[0-9]+\.[0-9]+(\.[0-9]+)?' | head -n1
    fi
}

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
        
        # Check minimum version using version_compare
        if version_compare "$CMAKE_VERSION" "$REQUIRED_CMAKE_VERSION"; then
            print_success "CMake version is sufficient (>= $REQUIRED_CMAKE_VERSION)"
            return 0
        else
            print_warning "CMake version $CMAKE_VERSION is too old (need >= $REQUIRED_CMAKE_VERSION)"
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
            
            if version_compare "$PYTHON_VERSION" "$REQUIRED_PYTHON_VERSION"; then
                print_success "Python found: $PYTHON_VERSION ($python_cmd)"
                PYTHON_CMD=$python_cmd
                return 0
            else
                print_warning "Python $PYTHON_VERSION is too old (need >= $REQUIRED_PYTHON_VERSION)"
            fi
        fi
    done
    
    print_warning "Suitable Python (>= $REQUIRED_PYTHON_VERSION) not found"
    return 1
}

# Check C++ compiler
check_compiler() {
    print_status "Checking for C++ compiler..."
    
    if [[ "$OS" == "macos" ]]; then
        if command_exists clang++; then
            CLANG_OUTPUT=$(clang++ --version 2>&1)
            CLANG_VERSION=$(extract_version "$CLANG_OUTPUT")
            if version_compare "$CLANG_VERSION" "$MIN_CLANG_VERSION"; then
                print_success "Clang++ found: $CLANG_VERSION (sufficient)"
                return 0
            else
                print_warning "Clang++ version $CLANG_VERSION is too old (need >= $MIN_CLANG_VERSION)"
            fi
        fi
        
        if command_exists g++; then
            GCC_OUTPUT=$(g++ --version 2>&1)
            GCC_VERSION=$(extract_version "$GCC_OUTPUT")
            if version_compare "$GCC_VERSION" "$MIN_GCC_VERSION"; then
                print_success "G++ found: $GCC_VERSION (sufficient)"
                return 0
            else
                print_warning "G++ version $GCC_VERSION is too old (need >= $MIN_GCC_VERSION)"
            fi
        fi
        
    elif [[ "$OS" == "linux" ]]; then
        if command_exists g++; then
            GCC_OUTPUT=$(g++ --version 2>&1)
            GCC_VERSION=$(extract_version "$GCC_OUTPUT")
            if version_compare "$GCC_VERSION" "$MIN_GCC_VERSION"; then
                print_success "G++ found: $GCC_VERSION (sufficient)"
                return 0
            else
                print_warning "G++ version $GCC_VERSION is too old (need >= $MIN_GCC_VERSION)"
            fi
        fi
        
        if command_exists clang++; then
            CLANG_OUTPUT=$(clang++ --version 2>&1)
            CLANG_VERSION=$(extract_version "$CLANG_OUTPUT")
            if version_compare "$CLANG_VERSION" "$MIN_CLANG_VERSION"; then
                print_success "Clang++ found: $CLANG_VERSION (sufficient)"
                return 0
            else
                print_warning "Clang++ version $CLANG_VERSION is too old (need >= $MIN_CLANG_VERSION)"
            fi
        fi
        
    elif [[ "$OS" == "windows" ]]; then
        if command_exists cl; then
            print_success "MSVC found"
            return 0
        elif command_exists g++; then
            GCC_OUTPUT=$(g++ --version 2>&1)
            GCC_VERSION=$(extract_version "$GCC_OUTPUT")
            if version_compare "$GCC_VERSION" "$MIN_GCC_VERSION"; then
                print_success "G++ found: $GCC_VERSION (sufficient)"
                return 0
            else
                print_warning "G++ version $GCC_VERSION is too old (need >= $MIN_GCC_VERSION)"
            fi
        fi
    fi
    
    print_warning "No suitable C++ compiler found"
    print_warning "Need: GCC $MIN_GCC_VERSION+, Clang $MIN_CLANG_VERSION+, or MSVC $MIN_MSVC_VERSION+"
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
    
    # Install missing packages with pinned versions
    BREW_PACKAGES=""
    
    if ! check_cmake; then
        # Try to install specific version, fall back to latest if not available
        print_status "Installing CMake version $PINNED_CMAKE_VERSION (or closest available)..."
        if ! brew install "cmake@$PINNED_CMAKE_VERSION" 2>/dev/null; then
            print_warning "Specific CMake version not available, installing latest..."
            BREW_PACKAGES="$BREW_PACKAGES cmake"
        fi
    fi
    
    if ! check_python; then
        BREW_PACKAGES="$BREW_PACKAGES python@$PINNED_PYTHON_VERSION"
    fi
    
    if [ -n "$BREW_PACKAGES" ]; then
        if ask_permission "Install missing packages via Homebrew:$BREW_PACKAGES?"; then
            print_status "Installing packages:$BREW_PACKAGES"
            brew install $BREW_PACKAGES
            
            # Link Python if needed
            if [[ "$BREW_PACKAGES" == *"python@"* ]]; then
                print_status "Linking Python $PINNED_PYTHON_VERSION..."
                brew link --force python@$PINNED_PYTHON_VERSION || true
            fi
        fi
    fi
}

install_linux_deps() {
    if [[ "$DISTRO" == "debian" ]]; then
        # Ubuntu/Debian
        APT_PACKAGES=""
        
        if ! check_cmake; then
            # Try to get a recent CMake version
            print_status "Checking for CMake PPA availability..."
            if ! apt-cache show cmake | grep -q "Version.*$PINNED_CMAKE_VERSION" 2>/dev/null; then
                print_status "Adding Kitware CMake PPA for newer versions..."
                if ask_permission "Add Kitware APT repository for newer CMake?"; then
                    sudo apt update
                    sudo apt install -y software-properties-common
                    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
                    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
                    sudo apt update
                fi
            fi
            APT_PACKAGES="$APT_PACKAGES cmake"
        fi
        
        if ! check_python; then
            # Try to install specific Python version
            PYTHON_PKG="python$PINNED_PYTHON_VERSION"
            if apt-cache show "$PYTHON_PKG" &>/dev/null; then
                APT_PACKAGES="$APT_PACKAGES $PYTHON_PKG $PYTHON_PKG-venv $PYTHON_PKG-pip"
            else
                APT_PACKAGES="$APT_PACKAGES python3 python3-venv python3-pip"
            fi
        fi
        
        if ! check_compiler; then
            # Install newer GCC if available
            GCC_PKG="gcc-$MIN_GCC_VERSION"
            if apt-cache show "$GCC_PKG" &>/dev/null; then
                APT_PACKAGES="$APT_PACKAGES $GCC_PKG g++-$MIN_GCC_VERSION"
            else
                APT_PACKAGES="$APT_PACKAGES build-essential g++"
            fi
        fi
        
        if [ -n "$APT_PACKAGES" ]; then
            if ask_permission "Install missing packages via apt:$APT_PACKAGES?"; then
                print_status "Updating package lists..."
                sudo apt update
                print_status "Installing packages:$APT_PACKAGES"
                sudo apt install -y $APT_PACKAGES
                
                # Set up alternatives for newer GCC if installed
                if [[ "$APT_PACKAGES" == *"gcc-$MIN_GCC_VERSION"* ]]; then
                    print_status "Setting up GCC $MIN_GCC_VERSION as default..."
                    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-$MIN_GCC_VERSION 100
                    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-$MIN_GCC_VERSION 100
                fi
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