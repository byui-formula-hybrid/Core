# Get the directory where this script is located
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "../..")
Set-Location $ProjectRoot

# Import shared functions (assumes shared.ps1 is in same folder)
$SharedPath = Join-Path $PSScriptRoot "shared.ps1"
. $SharedPath

Print-Header "Uninstall Script"

Uninstall-BuildArtifacts
Uninstall-PioArtifacts

Uninstall-PythonVirtualEnvironment