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

function Check-Elevation {
    $myWindowsID=[System.Security.Principal.WindowsIdentity]::GetCurrent()
    $myWindowsPrincipal=new-object System.Security.Principal.WindowsPrincipal($myWindowsID)
    $adminRole=[System.Security.Principal.WindowsBuiltInRole]::Administrator

    if (-not ($myWindowsPrincipal.IsInRole($adminRole))) {
        # Start-Process powershell -Verb RunAs -ArgumentList "-File `"$($MyInvocation.MyCommand.Path)`""
        Print-Error "The powershell script needs to be ran with administrative privileges!"
        exit 1
    }
}

function Validate-Directory {
    if ((Get-Location).Path -match '\s') {
        Print-Error "The current directory possesses spaces in the path, this will result in PlatformIO errors."
        Print-Warning "Move the repository to a different directory without spaces in it."
        Print-Warning "A typical good spot is in your Documents folder on your local machine."
        exit 1
    }
    # Directory is a valid one!
}

function User-Answer($ans) {
    if ($ans.Trim().ToLower() -eq "y" -or $ans.Trim().ToLower() -eq "yes") {
        $true
    } elseif ($ans.Trim().ToLower() -eq "n" -or $ans.Trim().ToLower() -eq "no") {
        return $false
    } else {
        Print-Warning "Couldn't interpret input assuming no"
        return $false
    }
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
    Print-Info "C++: $(if (Command-Exists 'g++') { "$checkBox" } else { "$redX" })"
    Print-Info "C: $(if (Command-Exists 'gcc') { "$checkBox" } else { "$redX" })"
    Print-Info "Python: $(if (Command-Exists 'python') { "$checkBox" } else { "$redX" })"
    Print-Info "Git: $(if (Command-Exists 'git') { "$checkBox" } else { "$redX" })"
    Print-Info "VS Code: $(if (Command-Exists 'code') { "$checkBox" } else { "$redX" })"
    Start-PythonVirtualEnvironment
    Print-Info "Python Virtual Environment: $(if ($env:VIRTUAL_ENV) { "$checkBox" } else { "$redX" })"
    Print-Info "PlatformIO: $(if (Command-Exists 'pio') { "$checkBox" } else { "$redX" })"
    Stop-PythonVirtualEnvironment
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
    if (-not (Set-Python)) { return 1}

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
            Refresh-Env
        }
    }

    if (Command-Exists "python") {
        # $cmdPath = (Get-Command "python").Path
        # Print-Info "Python path: $cmdPath"
        Print-Success "Python already installed"
        return
    } elseif (Command-Exists "python3") {
        Print-Success "Python 3 already installed"
        return
    }
    
    Print-Info "Installing Python..."
    if (Command-Exists "choco") {
        choco install python
        if ($LASTEXITCODE -eq 0) {
            Refresh-Env
        } else {
            Print-Error "Failed to install Python using choco"
            exit 1
        }
    } elseif (Command-Exists "winget") {
        winget install -e --id Python.Python.3.13 --scope machine --accept-source-agreements
        if ($LASTEXITCODE -eq 0) {
            Refresh-Env
        } else {
            Print-Error "Failed to install Python using winget"
            exit 1
        }
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

function Uninstall-Python {
    $pyPackages = winget list python | Select-String -Pattern '^Python' | ForEach-Object {
        $columns = $_ -split '\s{2,}'
        if ($columns.Count -ge 3) {
            [PSCustomObject]@{
                Name = $columns[0]
                Id = $columns[1]
                Version = $columns
            }
        }
    }

    $pyDir = ""
    if (Command-Exists "python") {
        $pyDir = ((Get-Command python).Source.split('\') | Select -SkipLast 1) -join '\'
        [Environment]::SetEnvironmentVariable("Path", ($env:Path -split ";" | Where-Object {$_ -notmatch "Python"}) -join ";", "User")
        [Environment]::SetEnvironmentVariable("Path", ($env:Path -split ";" | Where-Object {$_ -notmatch "Python"}) -join ";", "Machine")
        Remove-Item -Recurse -Force "$pyDir"
        return
    } elseif (Command-Exists "python3") {
        $pyDir = ((Get-Command python).Source.split('\') | Select -SkipLast 1) -join '\'
        [Environment]::SetEnvironmentVariable("Path", ($env:Path -split ";" | Where-Object {$_ -notmatch "Python3"}) -join ";", "User")
        [Environment]::SetEnvironmentVariable("Path", ($env:Path -split ";" | Where-Object {$_ -notmatch "Python3"}) -join ";", "Machine")
        Remove-Item -Recurse -Force "$pyDir"
        return
    } else {
        Print-Error "Python couldn't be found!"
        return
    }



    # Print-Info "Found these packages:"
    # $pyPackages | ForEach-Object {
    #     Print-Info "Name: $($_.Name), Id: $($_.Id), Version: $($_.Version)"
    # }
    # foreach ($pkg in $pyPackages) {
    #     Print-Info "Uninstalling $pkg"
    #     winget uninstall --name $pkg.Name --version $pkg.Version --silent
    # }
}

# ================================
# GIT, WINGET, MSYS2, MINGW
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
        if ($LASTEXITCODE -eq 0) {
            Refresh-Env
        } else {
            Print-Error "Failed to install Git"
            exit 1
        }
    } else {
        Print-Warning "Install Git manually: https://git-scm.com/download/win"
    }
}

function Install-Winget {
    if (Command-Exists winget) {
        Print-Success "winget already installed"
        return
    }

    # Get the current user-scope PATH variable
    $userPath = [System.Environment]::GetEnvironmentVariable("PATH", "User")
    $wingetPath = Join-Path $env:LOCALAPPDATA "Microsoft\WindowsApps\winget.exe"
	if (Test-Path $wingetPath) {
        # Check if the new path is already in the PATH variable to avoid duplicates
        if ($userPath -notlike "*$wingetPath*") {
            # Append the new path to the existing user PATH variable
            $newUserPath = "$userPath;$wingetPath"
            [System.Environment]::SetEnvironmentVariable("PATH", "$newUserPath", "User")
            Print-Success "Added winget to the environment"
        } else {
            Print-Error "Winget is already present in the environment variable but is inaccessable."
        }
	} else {
        # install it
        Add-AppxPackage https://github.com/microsoft/winget-cli/releases/latest/download/Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.msixbundle
        if ($LASTEXITCODE -eq 0) {
            Print-Success "Installed winget"
        } else {
            Print-Error "Failed to install winget"
            exit 1
        }
    }

    Refresh-Env
}

function Install-MSYS2 {
    if (Command-Exists "g++") {
        Print-Success "g++ compiler already installed"
        if (Command-Exists "gcc") {
            Print-Success "gcc compiler already installed"
            return
        }
        Print-Error "Compiler missing gcc but g++ was found, validate environment variables!"
        exit 1
    }
    if (Command-Exists "gcc") {
        Print-Error "Compiler missing g++ but gcc was found, validate environment variables!"
        exit 1
    }


    Print-Info "Installing g++ and gcc Compilers from MSYS2"
    if (Command-Exists "winget") {
        winget install --id=MSYS2.MSYS2 --location "C:\Program Files\msys64" --accept-source-agreements
        if ($LASTEXITCODE -eq 0) {Print-Info "Download complete, installing toolchain"}

        $ucrt64Args = @("pacman", "-S", "--noconfirm", "--needed", "base-devel", "mingw-w64-ucrt-x86_64-toolchain")
        Start-Process -FilePath "C:\Program Files\msys64\ucrt64.exe" -ArgumentList $ucrt64Args -Wait
        if ($LASTEXITCODE -eq 0) {Print-Info "Installation success, adding environment variables"}

        $machPath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
        $compilerPath = "C:\Program Files\msys64\ucrt64\bin"
        $newPath = "$machPath;$compilerPath"
        [System.Environment]::SetEnvironmentVariable("Path", "$NewPath", "Machine")
        if ($LASTEXITCODE -eq 0) {
            Refresh-Env
            Print-Success "Successfully installed the g++ and gcc compilers!"
        } else {
            Print-Error "Failed to install MSYS2 and the compilers: g++ and gcc"
            exit 1
        }
    } else {
        Print-Warning "Install MSYS2 manually: https://github.com/msys2/msys2-installer/releases/download/2024-12-08/msys2-x86_64-20241208.exe"
        Print-Warning "For latest release use: https://repo.msys2.org/distrib/x86_64/"
        Print-Warning "Select the latest exe that is not msys2-base"
    }
}

function Install-Mingw {
    if (Command-Exists "g++") {
        Print-Success "g++ compiler already installed"
        if (Command-Exists "gcc") {
            Print-Success "gcc compiler already installed"
            return
        }
        Print-Error "Compiler missing gcc but g++ was found, validate environment variables!"
        exit 1
    }
    if (Command-Exists "gcc") {
        Print-Error "Compiler missing g++ but gcc was found, validate environment variables!"
        exit 1
    }

    Print-Info "Installing g++ and gcc Compilers from MinGW"
    # if (Command-Exists "winget") {
    #     winget install --id=BrechtSanders.WinLibs.POSIX.UCRT --accept-source-agreements
    #     # Can add specific location, but not necessary for the CI
    #     if ($LASTEXITCODE -eq 0) {
    #         Refresh-Env
    #         Print-Success "Successfully installed the g++ and gcc compilers!"
    #     } else {
    #         Print-Error "Failed to install MinGW with compilers: g++ and gcc"
    #         exit 1
    #     }
    # } else {
    #     Print-Error "Failed to install g++ and gcc compilers!"
    #     exit 1
    # }
    # This is essentially a manual version of the commented code above, but is more stable
    if (Command-Exists "winget") {
        # Since winget is throwing errors in the CI attempting to go around that
        $mingwUrl = "https://github.com/brechtsanders/winlibs_mingw/releases/download/15.2.0posix-13.0.0-ucrt-r2/winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64ucrt-13.0.0-r2.zip"
        $mingwArchive = "$env:TEMP\mingw.zip"
        $mingwDir = "C:\Program Files\MinGW"
        if (Test-Path $mingwDir) {
            Print-Info "Remnants of a previous installation found and will be cleaned up!"
            Remove-Item -Recurse -Force $mingwDir
        }

        # Invoke-WebRequest -Uri $mingwUrl -OutFile $mingwArchive # This is REALLY SLOW
        $wc = New-Object System.Net.WebClient
        $wc.DownloadFile($mingwUrl, $mingwArchive)
        Print-Info "Download Complete!"
        New-Item -ItemType Directory -Force -Path $mingwDir | Out-Null
        if (Test-Path $mingwDir) {
            Print-Info "Directory created successfully"
        } else {
            Print-Error "Failed to create installation directory"
            exit 1
        }
        # Expand-Archive -Path $mingwArchive -DestinationPath $mingwDir # This is REALLY SLOW
        Add-Type -AssemblyName System.IO.Compression.FileSystem
        Print-Info "Extracting"
        [System.IO.Compression.ZipFile]::ExtractToDirectory("$mingwArchive", "$mingwDir")
        $mingwBinPath = Join-Path $mingwDir "mingw64\bin"
        $machPath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
        $newPath = "$machPath;$mingwBinPath"
        [System.Environment]::SetEnvironmentVariable("Path", "$NewPath", "Machine")
        
        if ($LASTEXITCODE -eq 0) {
            Refresh-Env
            Print-Success "Successfully installed the g++ and gcc compilers!"
        } else {
            Print-Error "Failed to install MinGW with compilers: g++ and gcc"
            exit 1
        }
    } else {
        Print-Error "Failed to install g++ and gcc compilers!"
        exit 1
    }
}

function Uninstall-MSYS2 {
    $msys2Dir = "C:\Program Files\msys64"
    $msys2Uninstaller = Join-Path $msys2Dir "uninstall.exe"
    if (Test-Path $msys2Uninstaller) {
        Start-Process $msys2Uninstaller -ArgumentList "purge --confirm-command" -Wait
    } else {
        Print-Error "Couldn't find the uninstaller for MSYS2"
    }
}

function Uninstall-MinGW {
    $machPath = [System.Environment]::GetEnvironmentVariable("PATH", "Machine")
    $mingwDir = "C:\Program Files\MinGW"
    $mingwBinPath = Join-Path $mingwDir "mingw64\bin"
    if ($machPath -like "*$mingwBinPath*") {
        Print-Info "Removing MinGW from environment variables"
        $machEntries = $machPath -split ";"
        $newEntries = $machEntries | where {$_ -ne $mingwBinPath}
        $newPathEntries = $newEntries -Join ';'
        [System.Environment]::SetEnvironmentVariable("PATH", $newPathEntries, "Machine")
    }

    if (Test-Path $mingwDir) {
        Remove-Item -Recurse -Force $mingwDir
        Print-Success "MinGW removed!"
    } else {
        Print-Warning "MinGW wasn't found"
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
        Refresh-Env
        Print-Success "PlatformIO installed successfully"
    } else {
        Print-Error "Failed to install PlatformIO"
    }
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
    if (User-Answer($confirm)) {
        Print-Info "Beginning VS Code installation"
    } else {
        Print-Warning "Skipping VS Code installation"
        return
    }

    if (Command-Exists choco) {
        choco install vscode -y
        if ($LASTEXITCODE -eq 0) {
            Refresh-Env
            Print-Success "Successfully installed VSCode"
        } else {
            Print-Error "Failed to install VSCode using choco"
            exit 1
        }
    } elseif (Command-Exists winget) {
        winget install -e --id Microsoft.VisualStudioCode --silent
        if ($LASTEXITCODE -eq 0) {
            Refresh-Env
            Print-Success "Successfully installed VSCode"
        } else {
            Print-Error "Failed to install VSCode using winget"
            exit 1
        }
    } else { Print-Warning "Install manually: https://code.visualstudio.com/download" }
}

function Install-VSCodeExtensions {
    if (-not (Command-Exists code)) {
        Print-Warning "VS Code not found!"
        return
    }

    $confirm = Read-Host "Install recommended VS Code extensions? (y/N)"
    if (User-Answer($confirm)) {
        Print-Info "Installing extentions"
    } else {
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
