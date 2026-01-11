# PlatformIO Setup Script for DIY SwitchBot
# This script configures PlatformIO for your environment
# Works on both corporate networks (with SSL interception) and home networks

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "   PlatformIO Environment Setup" -ForegroundColor Cyan
Write-Host "========================================`n" -ForegroundColor Cyan

# Find PlatformIO executable
$pioPath = "$env:USERPROFILE\.platformio\penv\Scripts\pio.exe"

if (-not (Test-Path $pioPath)) {
    Write-Host "[ERROR] PlatformIO not found at: $pioPath" -ForegroundColor Red
    Write-Host "Please install PlatformIO extension in VS Code/Cursor first." -ForegroundColor Yellow
    exit 1
}

Write-Host "[INFO] Found PlatformIO at: $pioPath" -ForegroundColor Green

# Test SSL connectivity to PlatformIO registry
Write-Host "`n[TEST] Checking network connectivity..." -ForegroundColor Yellow

$testScript = @"
import ssl
import urllib.request

try:
    # Test with SSL verification enabled
    response = urllib.request.urlopen('https://registry.platformio.org', timeout=10)
    print('SSL_OK')
except ssl.SSLError as e:
    print('SSL_ERROR')
except Exception as e:
    print('NETWORK_ERROR')
"@

$pythonPath = "$env:USERPROFILE\.platformio\penv\Scripts\python.exe"
$result = echo $testScript | & $pythonPath 2>$null

if ($result -eq "SSL_OK") {
    Write-Host "[OK] SSL verification works - you're on a normal network" -ForegroundColor Green
    Write-Host "[INFO] Enabling strict SSL verification..." -ForegroundColor Cyan
    & $pioPath settings set enable_proxy_strict_ssl yes 2>$null
    Write-Host "[OK] SSL verification: ENABLED" -ForegroundColor Green
}
elseif ($result -eq "SSL_ERROR") {
    Write-Host "[WARN] SSL interception detected (corporate network)" -ForegroundColor Yellow
    Write-Host "[INFO] Your network intercepts HTTPS traffic (common in corporate environments)" -ForegroundColor Yellow
    Write-Host "[INFO] Disabling strict SSL verification to allow PlatformIO to work..." -ForegroundColor Cyan
    & $pioPath settings set enable_proxy_strict_ssl no 2>$null
    Write-Host "[OK] SSL verification: DISABLED (required for your network)" -ForegroundColor Green
}
else {
    Write-Host "[WARN] Could not reach PlatformIO registry" -ForegroundColor Yellow
    Write-Host "[INFO] Check your internet connection" -ForegroundColor Yellow
    Write-Host "[INFO] Disabling SSL verification as a precaution..." -ForegroundColor Cyan
    & $pioPath settings set enable_proxy_strict_ssl no 2>$null
}

# Show current settings
Write-Host "`n[INFO] Current PlatformIO SSL Setting:" -ForegroundColor Cyan
& $pioPath settings get enable_proxy_strict_ssl

Write-Host "`n========================================" -ForegroundColor Cyan
Write-Host "   Setup Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "`nYou can now build the project with: pio run" -ForegroundColor White
Write-Host "Or use PlatformIO sidebar in VS Code/Cursor`n" -ForegroundColor White

