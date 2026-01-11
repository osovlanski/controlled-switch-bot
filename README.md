# 🤖 DIY SwitchBot

Build your own WiFi-enabled smart switch controller! Control any physical switch with voice commands (Alexa/Google Home), a mobile app, or AI integration.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)
![Status](https://img.shields.io/badge/status-active-success.svg)

---

## ✨ Features

- 🎤 **Voice Control** - Works with Alexa and Google Home via Sinric Pro
- 📱 **App Control** - Use Blynk app or built-in web interface
- 🌐 **WiFi Connected** - Control from anywhere on your network
- 🔌 **Easy Assembly** - No soldering required, just plug and play
- 💰 **Budget Friendly** - Total cost under $25 USD
- 🏠 **Smart Home Ready** - Integrates with IFTTT, Home Assistant, and more

---

## 📦 What You'll Need

| Component | Purpose | Cost |
|-----------|---------|------|
| ESP32 Dev Board | WiFi Microcontroller | ~$8 |
| SG90 Micro Servo | Mechanical actuator | ~$3 |
| Jumper Wires (3x) | Connections | ~$2 |
| 3M Command Strips | Mounting | ~$5 |
| USB Cable + Adapter | Power | ~$5 |

**Total: ~$23 USD**

👉 See [docs/HARDWARE.md](docs/HARDWARE.md) for detailed parts list with purchase links.

---

## 🚀 Quick Start

### 1. Wire It Up

```
ESP32 GPIO 13  →  Servo Orange (Signal)
ESP32 GND      →  Servo Brown (Ground)  
ESP32 VIN      →  Servo Red (Power)
```

### 2. Install Arduino IDE & Libraries

1. Download [Arduino IDE](https://www.arduino.cc/en/software)
2. Add ESP32 board support
3. Install libraries: `ESP32Servo`, `SinricPro`

### 3. Upload Code

```bash
# Clone this repository
git clone https://github.com/osovlanski/controlled-switch-bot.git
cd controlled-switch-bot

# Open in Arduino IDE
# File > Open > src/switchbot_simple/switchbot_simple.ino
```

### 4. Configure & Upload

Edit the WiFi credentials in the code, then upload to your ESP32.

### 5. Control Your Switch!

Open the IP address shown in Serial Monitor in your browser.

---

## 📁 Project Structure

```
controlled-switch-bot/
├── src/
│   ├── switchbot/              # Full version with Sinric Pro
│   │   ├── switchbot.ino       # Main code
│   │   └── config.h            # Configuration template
│   ├── switchbot_simple/       # Simple web server version
│   │   └── switchbot_simple.ino
│   └── switchbot_blynk/        # Blynk IoT version
│       └── switchbot_blynk.ino
├── docs/
│   ├── HARDWARE.md             # Wiring & assembly guide
│   └── SETUP.md                # Detailed setup instructions
├── .vscode/
│   ├── extensions.json         # Recommended extensions
│   └── settings.json           # Editor settings
├── .cursorrules                # AI assistant rules
├── .gitignore
└── README.md
```

---

## 🎮 Control Options

### Option 1: Simple Web Interface (No Account Required)
- Local network control
- Beautiful responsive UI
- No cloud dependency

### Option 2: Sinric Pro (Voice Control)
- Alexa integration
- Google Home integration
- Free tier available

### Option 3: Blynk IoT (Mobile App)
- Custom mobile dashboard
- Widgets and automation
- Free tier available

👉 See [docs/SETUP.md](docs/SETUP.md) for detailed instructions for each option.

---

## 🏗️ Mechanical Assembly

### For Rocker Switches (Light Switches)
The servo arm pushes the switch up. For pulling down, attach fishing line from switch to servo arm.

### For Push Buttons
Position the servo so the arm can press straight down on the button.

### Mounting
Use 3M Command Strips - strong enough for the servo, removable without damage.

👉 See [docs/HARDWARE.md](docs/HARDWARE.md) for 3D printable mounts and detailed assembly.

---

## 🔧 Configuration

### Servo Calibration

If the servo doesn't reach your switch, adjust these values:

```cpp
#define REST_ANGLE    90    // Idle position (try 80-100)
#define PUSH_ANGLE    150   // Push position (try 130-170)
#define PULL_ANGLE    30    // Pull position (try 20-50)
```

### WiFi Credentials

**Never commit credentials to git!** Copy `config.h` to `config_local.h` and edit there.

---

## 🤝 Integration Examples

### IFTTT
Create applets like:
- "When I arrive home, turn on the light"
- "At sunset, turn on the porch light"

### Home Assistant
Add to `configuration.yaml`:
```yaml
switch:
  - platform: rest
    name: SwitchBot
    resource: http://192.168.1.xxx/toggle
```

### AI (ChatGPT/Gemini)
Use Home Assistant's OpenAI Conversation integration for natural language control.

---

## 📜 License

MIT License - feel free to use, modify, and share!

---

## 🙏 Credits

- ESP32 Arduino Core by Espressif
- ESP32Servo library by Kevin Harrington
- Sinric Pro for cloud integration
- Inspired by commercial SwitchBot products

---

## ⚠️ Disclaimer

This is a DIY project. Use at your own risk. Not recommended for critical or safety-related switches. Always follow electrical safety guidelines.

