#!/bin/bash
# PlatformIO Setup Script for DIY SwitchBot
# This script configures PlatformIO for your environment
# Works on both corporate networks (with SSL interception) and home networks

echo ""
echo "========================================"
echo "   PlatformIO Environment Setup"
echo "========================================"
echo ""

# Find PlatformIO executable
PIO_PATH="$HOME/.platformio/penv/bin/pio"

if [ ! -f "$PIO_PATH" ]; then
    echo "[ERROR] PlatformIO not found at: $PIO_PATH"
    echo "Please install PlatformIO extension in VS Code/Cursor first."
    exit 1
fi

echo "[INFO] Found PlatformIO at: $PIO_PATH"

# Test SSL connectivity to PlatformIO registry
echo ""
echo "[TEST] Checking network connectivity..."

PYTHON_PATH="$HOME/.platformio/penv/bin/python"

RESULT=$($PYTHON_PATH -c "
import ssl
import urllib.request

try:
    response = urllib.request.urlopen('https://registry.platformio.org', timeout=10)
    print('SSL_OK')
except ssl.SSLError as e:
    print('SSL_ERROR')
except Exception as e:
    print('NETWORK_ERROR')
" 2>/dev/null)

if [ "$RESULT" = "SSL_OK" ]; then
    echo "[OK] SSL verification works - you're on a normal network"
    echo "[INFO] Enabling strict SSL verification..."
    $PIO_PATH settings set enable_proxy_strict_ssl yes 2>/dev/null
    echo "[OK] SSL verification: ENABLED"
elif [ "$RESULT" = "SSL_ERROR" ]; then
    echo "[WARN] SSL interception detected (corporate network)"
    echo "[INFO] Your network intercepts HTTPS traffic (common in corporate environments)"
    echo "[INFO] Disabling strict SSL verification to allow PlatformIO to work..."
    $PIO_PATH settings set enable_proxy_strict_ssl no 2>/dev/null
    echo "[OK] SSL verification: DISABLED (required for your network)"
else
    echo "[WARN] Could not reach PlatformIO registry"
    echo "[INFO] Check your internet connection"
    echo "[INFO] Disabling SSL verification as a precaution..."
    $PIO_PATH settings set enable_proxy_strict_ssl no 2>/dev/null
fi

# Show current settings
echo ""
echo "[INFO] Current PlatformIO SSL Setting:"
$PIO_PATH settings get enable_proxy_strict_ssl

echo ""
echo "========================================"
echo "   Setup Complete!"
echo "========================================"
echo ""
echo "You can now build the project with: pio run"
echo "Or use PlatformIO sidebar in VS Code/Cursor"
echo ""

