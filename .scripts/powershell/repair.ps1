# Get the directory where this script is located
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "../..")
Set-Location $ProjectRoot

# Import shared functions (assumes shared.ps1 is in same folder)
$SharedPath = Join-Path $PSScriptRoot "shared.ps1"
. $SharedPath

Print-Header "Repair Script"
Print-Info "Not meant for github CI"
Validate-Directory
Check-Elevation

$Clear_Cache = $false

# Parse command-line arguments to support --ci / -c and help
if ($args.Count -gt 0) {
	for ($i = 0; $i -lt $args.Count; $i++) {
		switch ($args[$i]) {
			'-c' { $Clear_Cache = $true; break }
			'--cc'   { $Clear_Cache = $true; break }
			'-h' {
				Write-Host "Usage: repair.ps1 [OPTIONS]`n"
				Write-Host "Options:"
                Write-Host "  -c, --cc     Clears the platformIO cache for the ESP32"
				Write-Host "  -h, --help            Show this help message"
				exit 0
			}
			'--help' {
				Write-Host "Usage: repair.ps1 [OPTIONS]`n"
				Write-Host "Options:"
                Write-Host "  -c, --cc     Clears the platformIO cache for the ESP32"
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

Print-Info "If any have an X it may be your problem, try running the installer script again:"
Print-Info ".\.scripts\powershell\install.ps1"
Show-ProjectInfo

if ($Clear_Cache) {
    $PioPath = Join-Path $env:USERPROFILE ".platformio\packages\tool-esptoolpy"
    if (Test-Path $PioPath) {
        Remove-Item -Recurse -Force $PioPath
        Print-Success "PlatformIO cache cleared!"
        Print-Info "Try building again!"
    } else {
        Print-Warning "No cache deteccted!"
    }
}