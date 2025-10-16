# Get the directory where this script is located, these variables must come before the powershell script is imported for the imported script to access them
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "../..")
Set-Location $ProjectRoot
$PythonCmd = ""

# Import shared functions (assumes shared.ps1 is in same folder)
$SharedPath = Join-Path $PSScriptRoot "shared.ps1"
. $SharedPath

Print-Header("Build Script")

# Default values
$Environment = "esp32dev"
$Clean = $false
$Upload = $false
$Monitor = $false

function Show-Help {
    Write-Host "Usage: .\.scripts\powershell\build.ps1 [OPTIONS]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -e, --env ENVIRONMENT    Build environment (esp32dev, native)"
    Write-Host "                           Note: 'native' is for testing only, use .\scripts\test.ps1"
    Write-Host "  -c, --clean              Clean build files before building"
    Write-Host "  -u, --upload             Upload to device after building"
    Write-Host "  -m, --monitor            Start serial monitor after upload"
    Write-Host "  -h, --help               Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\.scripts\powershell\build.ps1                       # Build for ESP32"
    Write-Host "  .\.scripts\powershell\build.ps1 --clean               # Clean build for ESP32"
    Write-Host "  .\.scripts\powershell\build.ps1 --upload              # Build and upload to ESP32"
    Write-Host "  .\.scripts\powershell\build.ps1 --upload --monitor    # Build, upload, and monitor"
    Write-Host ""
    Write-Host "For testing:"
    Write-Host "  .\scripts\powershell\test.ps1                # Run unit tests (native environment)"
}

# Parse command-line arguments
for ($i = 0; $i -lt $args.Length; $i++) {
    switch ($args[$i]) {
        '-e' { $Environment = $args[$i + 1]; $i++ }
        '--env' { $Environment = $args[$i + 1]; $i++ }
        '-c' { $Clean = $true }
        '--clean' { $Clean = $true }
        '-u' { $Upload = $true }
        '--upload' { $Upload = $true }
        '-m' { $Monitor = $true }
        '--monitor' { $Monitor = $true }
        '-h' { Show-Help; exit 0 }
        '--help' { Show-Help; exit 0 }
        '--ci' {
            # Continuous Integration mode for Github pipeline
            $Environment = 'esp32dev'
        }
        default {
            Print-Error "Unknown option: $($args[$i])"
            Write-Host "Use -h or --help for usage information"
            exit 1
        }
    }
}


Start-PythonVirtualEnvironment

# Validate environment
switch ($Environment) {
    'esp32dev' { Print-Info "Choosing esp32dev environment!" }
    'native' {
        Print-Warning "Native environment is for testing only"
        Print-Info "Use '.\scripts\powershell\test.ps1' to run tests"
        Print-Info "Use '--env esp32dev' for building"
        exit 1
    }
    default {
        Print-Error "Invalid environment: $Environment"
        Print-Error "Valid environments: esp32dev"
        Print-Error "For testing, use: .\scripts\powershell\test.ps1"
        exit 1
    }
}

# Show build info
Print-Info "Environment: $Environment"
Print-Info ("Clean build: " + ($(if ($Clean) { "Yes" } else { "No" })))
Print-Info ("Upload: " + ($(if ($Upload) { "Yes" } else { "No" })))
Print-Info ("Monitor: " + ($(if ($Monitor) { "Yes" } else { "No" })))

# Clean if requested
if ($Clean) {
    Print-Info "Cleaning build files..."
    if (pio run -e $Environment -t clean) {
        Print-Success "Clean completed"
    } else {
        Print-Error "Clean failed"
        exit 1
    }
}

# Build
Print-Info "Building for $Environment..."
pio run -e $Environment
if ($LASTEXITCODE -eq 0) {
    Print-Success "Build successful!"
} else {
    Print-Error "Build failed!"
    exit 1
}

# Upload if requested
if ($Upload) {
    if ($Environment -eq "esp32dev") {
        Print-Info "Uploading to ESP32..."
        if (pio run -e $Environment -t upload) {
            Print-Success "Upload successful!"
        } else {
            Print-Error "Upload failed!"
            exit 1
        }
    } else {
        Print-Warning "Upload only available for esp32dev environment"
    }
}

# Monitor if requested
if ($Monitor) {
    if ($Environment -eq "esp32dev") {
        Print-Info "Starting serial monitor..."
        Print-Warning "Press Ctrl+C to exit monitor"
        pio device monitor -e $Environment
    } else {
        Print-Warning "Monitor only available for esp32dev environment"
    }
}

Stop-PythonVirtualEnvironment
