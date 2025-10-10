<#
.SYNOPSIS
    Sets up Git Bash for any Windows user.
.DESCRIPTION
    - Checks if Git Bash is installed.
    - Installs Git using winget or choco if not found.
    - Adds Git Bash to the system PATH if necessary.
    - Optionally sets Git Bash as the default shell in Windows Terminal.
#>

# Ensure the script runs with admin privileges
if (-not ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(`
    [Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "Restarting script as Administrator..." -ForegroundColor Yellow
    Start-Process powershell "-ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    exit
}

Write-Host "=== Git Bash Setup ===" -ForegroundColor Cyan

# Step 1: Check if Git Bash exists
$gitPath = (Get-Command git.exe -ErrorAction SilentlyContinue)?.Source
if (-not $gitPath) {
    Write-Host "Git not found. Installing Git..." -ForegroundColor Yellow

    # Try using winget first
    if (Get-Command winget -ErrorAction SilentlyContinue) {
        winget install --id Git.Git -e --source winget --accept-package-agreements --accept-source-agreements
    }
    elseif (Get-Command choco -ErrorAction SilentlyContinue) {
        choco install git -y
    }
    else {
        Write-Host "Neither winget nor choco found. Please install Git manually:" -ForegroundColor Red
        Write-Host "https://git-scm.com/download/win"
        exit 1
    }

    # Refresh path
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
    $gitPath = (Get-Command git.exe -ErrorAction SilentlyContinue)?.Source
}

# Step 2: Locate Git Bash
$gitBashPath = "C:\Program Files\Git\bin\bash.exe"
if (-not (Test-Path $gitBashPath)) {
    $gitBashPath = "C:\Program Files\Git\git-bash.exe"
}

if (-not (Test-Path $gitBashPath)) {
    Write-Host "Git Bash executable not found after installation." -ForegroundColor Red
    exit 1
}

Write-Host "Git Bash found at: $gitBashPath" -ForegroundColor Green

# Step 3: Add Git Bash to PATH if not present
$pathEntries = [System.Environment]::GetEnvironmentVariable("Path", "User") -split ';'
$gitBinDir = Split-Path $gitBashPath

if ($pathEntries -notcontains $gitBinDir) {
    Write-Host "Adding Git Bash to PATH..." -ForegroundColor Yellow
    $newPath = ($pathEntries + $gitBinDir) -join ';'
    [System.Environment]::SetEnvironmentVariable("Path", $newPath, "User")
}

# Step 4: Set Git Bash as default in Windows Terminal (if present)
# $terminalSettings = "$env:LOCALAPPDATA\Packages\Microsoft.WindowsTerminal_8wekyb3d8bbwe\LocalState\settings.json"
# if (Test-Path $terminalSettings) {
#     Write-Host "Setting Git Bash as default shell in Windows Terminal..." -ForegroundColor Yellow

#     $json = Get-Content $terminalSettings | Out-String | ConvertFrom-Json

#     # Find Git Bash profile or create one
#     $gitProfile = $json.profiles.list | Where-Object { $_.name -eq "Git Bash" }
#     if (-not $gitProfile) {
#         $gitProfile = [PSCustomObject]@{
#             name = "Git Bash"
#             commandline = "`"$gitBashPath`" --login -i"
#             icon = "C:\\Program Files\\Git\\mingw64\\share\\git\\git-for-windows.ico"
#             hidden = $false
#         }
#         $json.profiles.list += $gitProfile
#     }

#     # Set as default
#     $json.profiles.default = "Git Bash"

#     # Save back
#     $json | ConvertTo-Json -Depth 5 | Set-Content $terminalSettings -Encoding UTF8
#     Write-Host "Windows Terminal default shell set to Git Bash." -ForegroundColor Green
# } else {
#     Write-Host "Windows Terminal not detected. Skipping Terminal setup." -ForegroundColor DarkYellow
# }

Write-Host "`n✅ Git Bash setup complete!" -ForegroundColor Cyan
Write-Host "You may need to restart your terminal or log out for changes to apply."
