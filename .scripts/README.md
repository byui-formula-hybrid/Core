# ESP32 Formula Hybrid Core Library - Scripts

This folder contains cross-platform scripts for managing the project, except Windows, see the powershell folder for that or use the Git Bash installation for Windows then these apply.

## Scripts

### Install Dependencies
Installs all required software for development:
- Python 3
- Git
- PlatformIO Core
- VS Code (optional)
- Project dependencies

**Usage:**
```bash
.scripts/bash/install.sh
```

```powershell
.scripts/powershell/install.ps1
```

**Supported Platforms:**
- macOS (via Homebrew)
- Linux (via apt, yum, dnf, pacman)
- Windows (via chocolatey, winget)

### `uninstall.sh` - Remove Dependencies
Removes PlatformIO and project build files (keeps Python, Git, VS Code).

**Usage:**
```bash
.scripts/uninstall.sh
```

```powershell
.scripts/powershell/uninstall.ps1
```

## Cross-Platform Support

These scripts work on:
- **macOS** - Uses Homebrew for package management
- **Linux** - Supports apt (Ubuntu/Debian), yum/dnf (RHEL/Fedora), pacman (Arch)
- **Windows** - Supports chocolatey and winget package managers

The scripts automatically detect your operating system and use the appropriate package manager.