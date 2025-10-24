# Get the directory where this script is located
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "../..")
Set-Location $ProjectRoot

# Import shared functions (assumes shared.ps1 is in same folder)
$SharedPath = Join-Path $PSScriptRoot "shared.ps1"
. $SharedPath

Print-Header "Uninstall Script"
Check-Elevation

# Default: not running in CI
$CI_MODE = $false

# Parse command-line arguments to support --ci / -c and help
if ($args.Count -gt 0) {
	for ($i = 0; $i -lt $args.Count; $i++) {
		switch ($args[$i]) {
			'--ci' { $CI_MODE = $true; break }
			'-c'   { $CI_MODE = $true; break }
			'-h' {
				Write-Host "Usage: install.ps1 [OPTIONS]`n"
				Write-Host "Options:"
				Write-Host "  --ci, -c               Setup for CI environment"
				Write-Host "  -h, --help             Show this help message"
				exit 0
			}
			'--help' {
				Write-Host "Usage: install.ps1 [OPTIONS]`n"
				Write-Host "Options:"
				Write-Host "  --ci, -c               Setup for CI environment"
				Write-Host "  -h, --help             Show this help message"
				exit 0
			}
			default {
				if (Get-Command Print-Error -ErrorAction SilentlyContinue) {
					Print-Error "Unknown option: $($args[$i])"
				} else {
					Write-Error "Unknown option: $($args[$i])"
				}
				Write-Host "Use -h or --help for usage information"
				exit 1
			}
		}
	}
}

Uninstall-BuildArtifacts
Uninstall-PioArtifacts

Uninstall-PythonVirtualEnvironment

if ($CI_MODE) {
    Uninstall-MinGW
    Uninstall-Python
    # Uninstall-PlatformIO
} else {
    # ask if the user wants tools removed
    $confirm = Read-Host "Do you want to uninstall Python? (y/N)"
	if (User-Answer($confirm)) {
        Uninstall-Python
	} else {
        Print-Warning "Skipping Python uninstallation"
	}

    $mingwDir = "C:\Program Files\MinGW"
    if (Test-Path $mingwDir) {
        $confirm = Read-Host "Do you want to uninstall MinGW? (y/N)"
		if (User-Answer($confirm)) {
            Uninstall-MinGW
		} else {
            Print-Warning "Skipping MinGW uninstallation"
		}
    }
}

Refresh-Env