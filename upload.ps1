# Helper script to upload firmware by killing blocking processes first
Write-Host "Killing Python processes that might be holding COM3..." -ForegroundColor Yellow
Get-Process python -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
Start-Sleep -Milliseconds 500

Write-Host "Uploading firmware..." -ForegroundColor Green
& pio run --target upload --environment esp32dev
