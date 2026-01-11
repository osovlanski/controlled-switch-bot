/**
 * Configuration file for DIY SwitchBot
 * 
 * IMPORTANT: Copy this file to 'config_local.h' and update with your credentials.
 * Never commit your actual credentials to git!
 * 
 * @author osovlanski
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// WIFI CREDENTIALS
// ============================================================================
// Replace with your WiFi network name and password
#define WIFI_SSID         "YOUR_WIFI_NAME"
#define WIFI_PASS         "YOUR_WIFI_PASSWORD"

// ============================================================================
// SINRIC PRO CREDENTIALS
// ============================================================================
// Get these from https://sinric.pro after creating a free account:
// 1. Create an account at sinric.pro
// 2. Add a new device of type "Switch"
// 3. Copy the credentials below

#define APP_KEY           "YOUR_SINRIC_APP_KEY"
#define APP_SECRET        "YOUR_SINRIC_APP_SECRET"
#define SWITCH_ID         "YOUR_DEVICE_ID"

// ============================================================================
// DEVICE SETTINGS (Optional customization)
// ============================================================================
// Device name shown in Alexa/Google Home
#define DEVICE_NAME       "SwitchBot"

// Rate limiting to prevent accidental rapid triggering
#define MIN_TRIGGER_INTERVAL_MS  1000

#endif // CONFIG_H

