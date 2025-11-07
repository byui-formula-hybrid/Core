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


if ($CI_MODE) {
    Uninstall-MinGW
	Start-PythonVirtualEnvironment
    Uninstall-PlatformIO
	Stop-PythonVirtualEnvironment
	Uninstall-PythonVirtualEnvironment
    Uninstall-Python
} else {
    $confirm = Read-Host "Do you want to uninstall Python? -Note: This will remove platformIO too (y/N)"
	if (User-Answer($confirm)) {
		# Currently some issues exist
		Start-PythonVirtualEnvironment
		Uninstall-PlatformIO
		Stop-PythonVirtualEnvironment
		Uninstall-PythonVirtualEnvironment
		Uninstall-Python
	} else {
        Print-Warning "Skipping Python uninstallation"
	}

    $mingwDir = "C:\Program Files\MinGW"
	$msys2Dir = "C:\Program Files\msys64"
    if (Test-Path $mingwDir) {
        $confirm = Read-Host "Do you want to uninstall MinGW? (y/N)"
		if (User-Answer($confirm)) {
            Uninstall-MinGW
		} else {
            Print-Warning "Skipping MinGW uninstallation"
		}
    } elseif (Test-Path $msys2Dir) {
		$confirm = Read-Host "Do you want to uninstall MSYS2? (y/N)"
		if (User-Answer($confirm)) {
			Uninstall-MSYS2
		} else {
			Print-Warning "Skipping MSYS2 uninstallation"
		}
	}
}

Refresh-Env