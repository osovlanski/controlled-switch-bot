# Setup Guide - DIY SwitchBot

Complete step-by-step instructions to get your SwitchBot running.

## 🛠️ Prerequisites

### Software Requirements

1. **Arduino IDE** (v2.0+ recommended)
   - Download: https://www.arduino.cc/en/software

2. **OR PlatformIO** (VS Code extension)
   - Better for experienced developers
   - Install via VS Code extensions

### Required Libraries

Install these via Arduino IDE Library Manager (`Sketch > Include Library > Manage Libraries`):

| Library | Author | Version |
|---------|--------|---------|
| ESP32Servo | Kevin Harrington | Latest |
| SinricPro | Boris Jaeger | Latest (for cloud version) |
| Blynk | Blynk Inc. | Latest (for Blynk version) |

---

## 📝 Arduino IDE Setup

### Step 1: Add ESP32 Board Support

1. Open Arduino IDE
2. Go to `File > Preferences`
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to `Tools > Board > Boards Manager`
5. Search for "ESP32" and install "ESP32 by Espressif Systems"

### Step 2: Select Your Board

1. Go to `Tools > Board > ESP32 Arduino`
2. Select "ESP32 Dev Module" (or your specific board)

### Step 3: Configure Board Settings

```
Board:          ESP32 Dev Module
Upload Speed:   921600
CPU Frequency:  240MHz (WiFi/BT)
Flash Mode:     QIO
Flash Size:     4MB (32Mb)
Partition:      Default 4MB with spiffs
PSRAM:          Disabled
```

---

## 🌐 Choose Your Control Method

### Option A: Simple Web Server (No Account Needed)

Best for: Local network control, testing, privacy-conscious users

1. Open `src/switchbot_simple/switchbot_simple.ino`
2. Update WiFi credentials:
   ```cpp
   const char* WIFI_SSID = "YourWiFiName";
   const char* WIFI_PASS = "YourWiFiPassword";
   ```
3. Upload to ESP32
4. Open Serial Monitor (115200 baud)
5. Note the IP address shown
6. Open `http://[IP_ADDRESS]/` in your browser

### Option B: Sinric Pro (Alexa/Google Home)

Best for: Voice control, smart home integration

#### Create Sinric Pro Account

1. Go to https://sinric.pro
2. Create a free account
3. Go to "Devices" > "Add Device"
4. Create a new "Switch" device
5. Note your:
   - App Key
   - App Secret
   - Device ID

#### Configure Code

1. Copy `src/switchbot/config.h` to `src/switchbot/config_local.h`
2. Edit `config_local.h`:
   ```cpp
   #define WIFI_SSID     "YourWiFiName"
   #define WIFI_PASS     "YourWiFiPassword"
   #define APP_KEY       "your-app-key-here"
   #define APP_SECRET    "your-app-secret-here"
   #define SWITCH_ID     "your-device-id-here"
   ```
3. Update `switchbot.ino` to include `config_local.h` instead of `config.h`
4. Upload to ESP32

#### Link to Voice Assistants

**Alexa:**
1. Open Alexa app
2. Go to Skills & Games
3. Search "Sinric Pro"
4. Enable and link account
5. Discover devices

**Google Home:**
1. Open Google Home app
2. Add device > Works with Google
3. Search "Sinric Pro"
4. Link account
5. Discover devices

### Option C: Blynk IoT

Best for: Mobile app control, custom dashboards

#### Create Blynk Account

1. Download Blynk app (iOS/Android)
2. Create account at https://blynk.cloud
3. Create new template:
   - Name: "SwitchBot"
   - Hardware: ESP32
   - Connection: WiFi
4. Add a Switch widget on V0
5. Copy your Template ID and Auth Token

#### Configure Code

1. Open `src/switchbot_blynk/switchbot_blynk.ino`
2. Update credentials:
   ```cpp
   #define BLYNK_TEMPLATE_ID   "TMPLxxxxxxxx"
   #define BLYNK_TEMPLATE_NAME "SwitchBot"
   #define BLYNK_AUTH_TOKEN    "YourAuthToken"

   const char* WIFI_SSID = "YourWiFiName";
   const char* WIFI_PASS = "YourWiFiPassword";
   ```
3. Upload to ESP32

---

## 🔌 Hardware Assembly

See [HARDWARE.md](./HARDWARE.md) for detailed wiring and assembly instructions.

### Quick Wiring Reference

```
ESP32 GPIO 13  →  Servo Orange (Signal)
ESP32 GND      →  Servo Brown (Ground)
ESP32 VIN      →  Servo Red (Power)
```

---

## ✅ Testing

### Serial Monitor Test

1. Open Serial Monitor (115200 baud)
2. You should see:
   ```
   ========================================
      DIY SwitchBot - Starting Up...
   ========================================

   [SERVO] Initializing servo...
   [SERVO] Attached to GPIO 13, initial angle: 90
   [WIFI] Connecting to YourWiFi...
   [WIFI] Connected!
   [WIFI] IP Address: 192.168.1.xxx
   [READY] SwitchBot is ready for commands!
   ```

### Physical Test

1. Verify servo moves to rest position on startup
2. Trigger the switch via app/web/voice
3. Confirm servo pushes and returns

### Calibration

If the servo doesn't reach the switch properly, adjust these values:

```cpp
#define REST_ANGLE    90    // Try 80-100
#define PUSH_ANGLE    150   // Try 130-170
#define PULL_ANGLE    30    // Try 20-50
```

---

## 🔒 Security Notes

1. **Never commit credentials** - Keep `config_local.h` in `.gitignore`
2. **Use strong WiFi password** - Your switch is only as secure as your network
3. **Enable router firewall** - Block external access to ESP32
4. **Regular updates** - Update Sinric/Blynk libraries periodically

---

## 🆘 Troubleshooting

### Upload Fails

1. Hold BOOT button while uploading
2. Try different USB cable
3. Install CH340/CP2102 drivers if needed

### WiFi Won't Connect

1. Verify credentials (case-sensitive!)
2. Check 2.4GHz network (ESP32 doesn't support 5GHz)
3. Move closer to router

### Servo Doesn't Move

1. Check wiring connections
2. Verify GPIO pin in code matches wiring
3. Try external 5V power supply

### Voice Commands Not Working

1. Re-discover devices in Alexa/Google Home
2. Check Sinric Pro dashboard for connection status
3. Verify device is online in Sinric Pro

---

## 📱 Voice Command Examples

Once linked to Alexa or Google Home:

```
"Alexa, turn on SwitchBot"
"Alexa, turn off SwitchBot"

"Hey Google, turn on SwitchBot"
"Hey Google, turn off SwitchBot"
```

Rename the device in Sinric Pro for custom names:
```
"Alexa, turn on bedroom light"
"Hey Google, turn off the lamp"
```




