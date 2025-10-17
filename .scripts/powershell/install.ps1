# Get the directory where this script is located
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "../..")
Set-Location $ProjectRoot

# Import shared functions (assumes shared.ps1 is in same folder)
$SharedPath = Join-Path $PSScriptRoot "shared.ps1"
. $SharedPath

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

Print-Header "Installation Script"

Install-Winget

if (-not $CI_MODE) {
	Install-Git
}

Install-Python
if (-not $CI_MODE) {
	Install-MSYS2
} else {
	# Print-Warning "Github Pipeline CI has storage limits, it will not let you install the g++ compiler because it exceeds 500 MB"
	Print-Header "System Information:"
	$totRam = (Get-WmiObject -Class Win32_computerSystem).TotalPhysicalMemory / 1GB
	Print-Info "Ram: $totRam"
	$storageInfo = Get-PSDrive -PSProvider 'FileSystem' | Select-Object Name, @{Name='Used(MB)';Expression={[math]::Round($_.Used / 1MB)}}, @{Name='Free(MB)';Expression={[math]::Round($_.Free / 1MB)}}, Provider | Format-Table -AutoSize
	Print-Info "Storage info: $torageInfo"
	# Install-Mingw
}
if (-not $CI_MODE) {
	Install-VSCode
}

Set-Python
Start-PythonVirtualEnvironment
Install-PlatformIO
Install-PythonPackages
Stop-PythonVirtualEnvironment

if (-not $CI_MODE) {
	Install-VSCodeExtensions
}

Show-ProjectInfo

Print-Success "Installation complete!"