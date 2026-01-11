# Hardware Guide - DIY SwitchBot

This guide covers all hardware aspects of building your DIY SwitchBot.

## 📦 Bill of Materials (BOM)

| Component | Purpose | Quantity | Estimated Cost | Where to Buy |
|-----------|---------|----------|----------------|--------------|
| ESP32 Dev Board | Microcontroller with WiFi | 1 | $5-15 | [Amazon](https://amazon.com/s?k=esp32+dev+board), [AliExpress](https://aliexpress.com/w/wholesale-esp32-dev-board.html) |
| SG90 Micro Servo | Mechanical actuator | 1 | $2-5 | [Amazon](https://amazon.com/s?k=sg90+servo), [Adafruit](https://adafruit.com) |
| Jumper Wires | Connections | 3+ | $3-5 | [SparkFun](https://sparkfun.com), [Amazon](https://amazon.com/s?k=jumper+wires) |
| Micro USB Cable | Power & Programming | 1 | $3-5 | Any local store |
| 3M Command Strips | Mounting | 1 pack | $5-8 | [Amazon](https://amazon.com/s?k=3m+command+strips) |
| **Optional:** 3.7V LiPo Battery | Portable power | 1 | $5-10 | [Adafruit](https://adafruit.com), [Amazon](https://amazon.com/s?k=3.7v+lipo+battery) |

**Total Estimated Cost: $20-50 USD**

---

## 🔌 Wiring Diagram

### ESP32 to SG90 Servo Connection

```
┌─────────────────────────────────────────────────────────────┐
│                         ESP32 Board                          │
│                                                              │
│    3.3V ─────────────────────────────────────────────────   │
│                                                              │
│    VIN (5V) ─────────────────┬───────────────────────────   │
│                              │                               │
│    GND ────────────────┬─────│───────────────────────────   │
│                        │     │                               │
│    GPIO 13 ───────┐    │     │                               │
│                   │    │     │                               │
└───────────────────│────│─────│───────────────────────────────┘
                    │    │     │
                    │    │     │
              ┌─────│────│─────│─────┐
              │     ▼    ▼     ▼     │
              │  Orange Brown Red    │
              │  (PWM) (GND) (VCC)   │
              │                      │
              │    SG90 SERVO        │
              │      ┌────┐          │
              │      │    │          │
              │      └────┘          │
              └──────────────────────┘
```

### Wire Colors (Standard SG90)

| Servo Wire | Color | Connect To |
|------------|-------|------------|
| Signal/PWM | Orange/Yellow | GPIO 13 |
| Ground | Brown/Black | GND |
| Power | Red | VIN (5V) |

### Pin Reference

```
ESP32 Pin    →    Servo Wire    →    Purpose
─────────────────────────────────────────────
GPIO 13      →    Orange        →    PWM Signal
GND          →    Brown         →    Ground
VIN/5V       →    Red           →    Power (5V)
```

---

## ⚡ Power Options

### Option 1: USB Power (Recommended for Testing)
- Use a 5V USB power adapter (phone charger works great)
- Minimum 500mA output recommended
- Most reliable option

### Option 2: USB Power Bank
- Great for temporary installations
- Choose a power bank that doesn't auto-shutoff on low current
- Some power banks turn off when the ESP32 is in low-power mode

### Option 3: LiPo Battery (Advanced)
- Use 3.7V LiPo battery with a charging module
- Connect to ESP32's battery pins if available
- Add a TP4056 charging module for USB charging

⚠️ **Power Warning**: If the servo "chatters" or moves erratically, your power source may be insufficient. Try a dedicated 5V power supply instead of USB from a computer.

---

## 🏗️ Mechanical Assembly

### Mounting the Servo

1. **Identify Your Switch Type**
   - **Rocker Switch**: Needs push AND pull mechanism
   - **Push Button**: Only needs push mechanism
   - **Toggle Switch**: Needs push at an angle

2. **Servo Horn Selection**
   - Use the single-arm horn (comes with SG90)
   - For extra grip, glue a small piece of rubber to the tip

3. **Positioning**
   - The servo shaft should be positioned to align with the switch
   - Allow 2-3cm clearance for the arm movement
   - The arm should reach the switch at ~150° angle

### Mounting Methods

#### Method A: 3D Printed Mount (Best)
Download from:
- [Thingiverse - SwitchBot Mount](https://www.thingiverse.com/search?q=switchbot+mount)
- [Printables - SG90 Light Switch](https://www.printables.com/search/models?q=sg90%20light%20switch)

#### Method B: DIY Bracket
```
Materials:
- Small plastic project box or thick cardboard
- Hot glue gun
- 3M Command Strips

Steps:
1. Cut a slot in the box for the servo to sit in
2. Hot glue the servo in place
3. Attach Command Strips to the back
4. Mount next to the switch
```

#### Method C: Direct Mount
```
1. Use the servo mounting tabs and small screws
2. Screw directly into a 3D printed plate
3. Attach plate with Command Strips
```

---

## 🔄 Servo Calibration

### Finding the Right Angles

The default angles may not work for your specific switch. Here's how to calibrate:

1. **Upload the simple web server version first**
2. **Access the web interface**
3. **Adjust angles in code:**

```cpp
#define REST_ANGLE    90    // Neutral - arm horizontal
#define PUSH_ANGLE    150   // Pushing - increase if not reaching
#define PULL_ANGLE    30    // Pulling - decrease if not reaching
```

### Calibration Tips

| Symptom | Solution |
|---------|----------|
| Arm doesn't reach switch | Increase PUSH_ANGLE |
| Arm overshoots switch | Decrease PUSH_ANGLE |
| Arm doesn't return far enough | Adjust REST_ANGLE |
| Pull doesn't work | Decrease PULL_ANGLE |

---

## 🎯 Switch Type Configurations

### Standard Light Switch (Rocker)

For rocker switches that flip up/down:

```cpp
// Push = flip up (ON)
#define PUSH_ANGLE  150

// Pull = flip down (OFF) - requires string attachment
#define PULL_ANGLE  30
```

**Pull Mechanism**: Attach fishing line or thin string from the switch to the servo arm. When the servo rotates to PULL_ANGLE, it will pull the switch back.

### Push Button

For momentary push buttons:

```cpp
// Only push is needed
#define PUSH_ANGLE  150
#define REST_ANGLE  90

void pushButton() {
    servo.write(PUSH_ANGLE);
    delay(200);  // Quick press
    servo.write(REST_ANGLE);
}
```

### Toggle Switch

For toggle switches (flip left/right):

```cpp
// Mount servo at an angle
#define PUSH_ANGLE  130
#define PULL_ANGLE  50
```

---

## 🔧 Troubleshooting

### Servo Issues

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| Servo doesn't move | Wrong pin or no power | Check wiring, verify GPIO pin in code |
| Servo chatters/vibrates | Insufficient power | Use dedicated 5V power supply |
| Servo moves erratically | Noise on signal line | Add 100nF capacitor between VCC and GND |
| Servo is weak | Drawing too much current | Use external power for servo, not ESP32's 5V |

### WiFi Issues

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| Won't connect | Wrong credentials | Double-check SSID and password |
| Disconnects frequently | Weak signal | Move ESP32 closer to router |
| Slow response | Network congestion | Check for interference |

### Power Issues

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| ESP32 resets when servo moves | Power brownout | Use better power supply, add capacitor |
| ESP32 won't boot | Voltage too low | Ensure 5V supply to VIN |
| Gets hot | Short circuit or overcurrent | Check wiring immediately! |

---

## 📐 Dimensions Reference

### ESP32 DevKit v1
- Length: 55mm
- Width: 28mm
- Height: 12mm (with pins)

### SG90 Micro Servo
- Length: 23mm
- Width: 12.2mm
- Height: 29mm (including shaft)
- Shaft to bottom: 15mm

### Total Assembly (Approximate)
- Recommended enclosure: 60mm x 40mm x 35mm minimum

