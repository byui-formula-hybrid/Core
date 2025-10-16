<#
Shared PowerShell functions for ESP32 Formula Hybrid Core Library scripts.
Equivalent to shared.sh for Windows / PowerShell users.
#>

# ================================
# COLORIZED OUTPUT HELPERS
# ================================

$checkBox = [char]0x2705
$redX = [char]0x274C
$infoBox = [char]0x2139
$warningSym = [char]0x26A0

function Print-Info($msg) {Write-Host "$infoBox  $msg" -ForegroundColor Blue}
function Print-Success($msg) { Write-Host "$checkBox  $msg" -ForegroundColor Green }
function Print-Warning($msg) { Write-Host "$warningSym  $msg" -ForegroundColor DarkYellow }
function Print-Error($msg)   { Write-Host "$redX  $msg" -ForegroundColor Red }

function Print-Header($msg) {
    Write-Host ("=" * $msg.Length)
    Write-Host "$msg" -ForegroundColor Cyan
    Write-Host ("=" * $msg.Length)
}

# ================================
# ENVIRONMENT MANAGEMENT
# ================================
function Refresh-Env {
    # Refresh system environment variables
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" +
                [System.Environment]::GetEnvironmentVariable("Path", "User")

    # Give time for environment changes before attempting to run next command
    Start-Sleep -Seconds 1
}

# ================================
# COMMAND CHECK
# ================================
function Command-Exists($cmd) {
    return [bool](Get-Command $cmd -ErrorAction SilentlyContinue)
}

# ================================
# PROJECT INFO
# ================================
function Show-ProjectInfo {
    Print-Header "ESP32 Formula Hybrid Core Library"
    Print-Info "Python: $(if (Command-Exists 'python') { "$checkBox" } else { "$redX" })"
    Print-Info "Git: $(if (Command-Exists 'git') { "$checkBox" } else { "$redX" })"
    Print-Info "PlatformIO: $(if (Command-Exists 'pio') { "$checkBox" } else { "$redX" })"
    Print-Info "VS Code: $(if (Command-Exists 'code') { "$checkBox" } else { "$redX" })"
}

# ================================
# PYTHON VIRTUAL ENV
# ================================
function Set-Python {
    if (Command-Exists "python") {
        $global:PythonCmd = "python"
        }
    elseif (Command-Exists "python3") {
        $global:PythonCmd = "python3"
        }
    else {
        Print-Error "Python not found! Please install Python first."
        return $false
    }
    return $true
}

function Run-Python($pyArgs) {
    switch($global:PythonCmd) {
        "python" {python @pyArgs}
        "python3" {python3 @pyArgs}
        default {
            Print-Error "Python error"
            Exit 1
        }
    }
}

function Start-PythonVirtualEnvironment {
    if (-not (Set-Python)) { return }

    $VenvPath = Join-Path $ProjectRoot ".venv"
    if (Test-Path $VenvPath) {
        & .\.venv\Scripts\Activate.ps1
        Print-Info "Activated existing Python virtual environment"
    } else {
        Run-Python @("-m", "venv", ".venv")
        if ($LASTEXITCODE -ne 0) {
            Print-Error "Failed to create Python virtual environment"
            return
        }
        & .\.venv\Scripts\Activate.ps1
        Print-Success "Created and activated new Python virtual environment"
    }
}

function Stop-PythonVirtualEnvironment {
    if ($env:VIRTUAL_ENV) {
        deactivate
        Print-Info "Deactivated Python virtual environment"
    } else {
        Print-Error "Failed to deactivate python virtual environment"
    }
}

# ================================
# PYTHON
# ================================
function Install-PythonPackages {
    $ReqPath = Join-Path $ProjectRoot "requirements.txt"
    if (-not (Test-Path $ReqPath)) {
        Print-Warning "requirements.txt not found, skipping installation"
        return
    }

    if (-not (Set-Python)) { return }
    Print-Info "Installing Python packages..."
    Run-Python @("-m", "pip", "install", "--upgrade", "pip")
    Run-Python @("-m", "pip", "install", "-r", "requirements.txt")
    if ($LASTEXITCODE -eq 0) {
        Print-Success "Python packages installed successfully"
    } else {
        Print-Error "Failed to install Python packages"
    }
}

function Install-Python() {
    # Check if Microsoft is dumb!!!
    $stubPath = "$env:LOCALAPPDATA\Microsoft\WindowsApps"
    $files = @("python.exe", "python3.exe")
    foreach ($f in $files) {
        $fPath = Join-Path $stubPath $f
        if (Test-Path $fPath) {
            Remove-Item -Path $fPath -Force
            Print-Warning "Microsofts stub python was found and was removed!"
        }
    }
    
    if (Command-Exists "python") {
        Print-Success "Python already installed"
        return
    } elseif (Command-Exists "python3") {
        Print-Success "Python 3 already installed"
        return
    }
    
    Print-Info "Installing Python..."
    if (Command-Exists "choco") {
        choco install python
        if ($LASTEXITCODE -eq 0) {Refresh-Env}
    } elseif (Command-Exists "winget") {
        winget install -e --id Python.Python.3.13 --scope machine
        if ($LASTEXITCODE -eq 0) {Refresh-Env}
    } else {
        Print-Warning "Install Python manually: https://www.python.org/downloads/windows/"
        Print-Warning "Using the 64-bit installer"
    }
}

function Uninstall-PythonVirtualEnvironment {
    if (-not (Set-Python)) { return }

    $VenvPath = Join-Path $ProjectRoot ".venv"
    if (Test-Path $VenvPath) {
        # Delete the .venv folder
        Remove-Item ".venv" -Recurse -Force
        Print-Success "Python virtual environment removed!"
    } else {
        Print-Warning "No Python virtual environment found to remove!"
    }
}

# ================================
# GIT
# ================================
function Install-Git {
    if (Command-Exists git) {
        Print-Success "Git already installed"
        return
    }

    Print-Info "Installing Git..."
    $installed = $false
    if (Command-Exists "choco") {
        choco install git -y
        if ($LASTEXITCODE -eq 0) {Refresh-Env}
    } elseif (Command-Exists "winget") {
        winget install --id Git.Git -e --silent --source winget
        if ($LASTEXITCODE -eq 0) {Refresh-Env}
    } else {
        Print-Warning "Install Git manually: https://git-scm.com/download/win"
    }
}

# ================================
# PLATFORMIO
# ================================
function Install-PlatformIO {
    if (Command-Exists "pio") {
        Print-Success "PlatformIO already installed"
        return
    }

    if (-not (Set-Python)) { return }
    Print-Info "Installing PlatformIO Core..."
    Run-Python @("-m", "pip", "install", "--upgrade", "platformio")
    if ($LASTEXITCODE -eq 0) {
        Print-Success "PlatformIO installed successfully"
        Refresh-Env
    } else {
        Print-Error "Failed to install PlatformIO"
    }

    # Possibly add platform io to environment variables
}

function Uninstall-PioArtifacts {
    $PioPath = Join-Path $ProjectRoot ".pio"
    if (Test-Path $PioPath) {
        Remove-Item -Recurse -Force ".pio"
        Print-Success "PlatformIO artifacts removed"
    } else {
        Print-Warning "No PlatformIO artifacts found"
    }
}

# ================================
# VS CODE
# ================================
function Install-VSCode {
    if (Command-Exists code) {
        Print-Success "VS Code already installed"
        return
    }

    $confirm = Read-Host "Do you want to install VS Code? (y/N)"
    if ($confirm -ne "y") {
        Print-Warning "Skipping VS Code installation"
        return
    }

    if (Command-Exists choco) {
        choco install vscode -y
        if ($LASTEXITCODE -eq 0) {Refresh-Env}
        }
    elseif (Command-Exists winget) {
        winget install -e --id Microsoft.VisualStudioCode --silent
        if ($LASTEXITCODE -eq 0) {Refresh-Env}
        }
    else { Print-Warning "Install manually: https://code.visualstudio.com/download" }
}

function Install-VSCodeExtensions {
    if (-not (Command-Exists code)) {
        Print-Warning "VS Code not found!"
        return
    }

    $confirm = Read-Host "Install recommended VS Code extensions? (y/N)"
    if ($confirm -ne "y") {
        Print-Warning "Skipping extension installation"
        return
    }

    $reqExtensions = @(
        "platformio.platformio-ide",
        "bierner.markdown-mermaid",
        "ms-python.python",
        "ms-vscode.cpptools-extension-pack"
    )

    $altExtensions = @(
        "yzhang.markdown-all-in-one",
        "GitHub.copilot",
        "redhat.vscode-yaml",
        "eamodio.gitlens",
        "hbenl.vscode-test-explorer",
        "oderwat.indent-rainbow",
        "aaron-bond.better-comments"
    )

    foreach ($ext in $reqExtensions) {
        Print-Info "Installing extension: $ext"
        code --install-extension $ext --force
    }
    Print-Success "VS Code extensions installed"
}

# ================================
# TESTS
# ================================
function Count-Tests {
    return (Get-ChildItem -Path .\test -Filter "test_main.cpp" -Recurse -ErrorAction SilentlyContinue | Measure-Object | Select-Object -ExpandProperty Count)
}

function List-TestModules {
    return (Get-ChildItem -Path .\test -Filter "test_main.cpp" -Recurse -ErrorAction SilentlyContinue | ForEach-Object {$_.Directory.Name.split("_")[1]} | Sort)
}

# ================================
# BUILD CLEANUP
# ================================
function Uninstall-BuildArtifacts {
    $BuildPath = Join-Path $ProjectRoot "build"
    if (Test-Path $BuildPath) {
        Remove-Item -Recurse -Force "build"
        Print-Success "Build artifacts removed"
    } else {
        Print-Warning "No build artifacts found"
    }
}
