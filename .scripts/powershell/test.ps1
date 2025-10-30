# Get the directory where this script is located
$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "../..")
Set-Location $ProjectRoot

# Parse command line arguments
$verbose=$false
$filter=""
$testResult = 1

# Import shared functions (assumes shared.ps1 is in same folder)
$SharedPath = Join-Path $PSScriptRoot "shared.ps1"
. $SharedPath

Print-Header "Test Suite"
Validate-Directory

# Function to run tests
function Run-Tests {
    Print-Info "Running native unit tests..."
    
    # Run tests with verbose output if requested
    $pioArgs = @("test", "-e", "native")
    if ($verbose) {
        $pioArgs += "-v"
    }
    
    pio @pioArgs
    # Write-Host "The Exit Code: $LASTEXITCODE /n/n"
    if ($LASTEXITCODE -eq 0) {
        Print-Success "All tests passed!"
    } else {
        Print-Error "At least one test failed!"
    }
    return
}

# Function to show test summary
function Show-Summary {
    Print-Header "Test Summary"
    
    # Count test files
    $test_count=Count-Tests
    Print-Info "Test suites found: $test_count"
    
    # List test modules
    Print-Info "Test modules:"
    List-TestModules | ForEach-Object {
        Write-Host "  * $_"
    }
}

function Show-Help {
    Write-Host "Usage: $0 [OPTIONS]"        
    Write-Host "Options:"
    Write-Host "  -v, --verbose    Show detailed test output"
    Write-Host "  -f, --filter     Run specific test module (e.g., test_core_module)"
    Write-Host "  -h, --help       Show this help message"
    Write-Host "Examples:"
    Write-Host "  $0                            # Run all tests"
    Write-Host "  $0 --verbose                  # Run all tests with verbose output"
    Write-Host "  $0 --filter test_core_module  # Run only core module tests"
}

# Parse command-line arguments
for ($i = 0; $i -lt $args.Length; $i++) {
    switch ($args[$i]) {
        '-v' {$verbose = $true; $i++ }
        '--verbose' { $verbose = $true; $i++ }
        '-f' { $i++; $filter = $args[$i] }
        '--filter' { $i++; $filter = $args[$i] }
        '-h' { Show-Help; exit 0 }
        '--help' { Show-Help; exit 0 }
        default {
            Print-Error "Unknown option: $($args[$i])"
            Write-Host "Use -h or --help for usage information"
            exit 1
        }
    }
}

# Show project info
Print-Info "Environment: native (unit testing only)"
Show-ProjectInfo

# Show test summary
Show-Summary

# Run specific test if filter is provided
if ($filter) {
    Print-Info "Running filtered tests: $filter"
    
    $pioArgs = @("test", "-e", "native", "--filter")
    if ($verbose) {
        $pioArgs += "-v"
    }
    

    pio $pioArgs
    if ($LASTEXITCODE -eq 0) {
        Print-Success "Filtered tests passed!"
    } else {
        Print-Error "Filtered tests failed!"
        exit 1
    }
    exit 0
} else {
    # Run all tests
    Run-Tests
    $testResult = $LASTEXITCODE
}


Print-Info "Quick commands:"
Write-Host "  Run all tests:     .scripts\powershell\test.ps1"
Write-Host "  Verbose output:    .scripts\powershell\test.ps1 --verbose"
Write-Host "  Specific module:   .scripts\powershell\test.ps1 --filter test_core_module"
Write-Host "  Build project:     .scripts\powershell\build.ps1"
Write-Host "  Clean build:       pio run -t clean"

# Exit with test result
if ($filter) {
    exit 0
} else {
    exit $testResult
}