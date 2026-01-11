/**
 * DIY SwitchBot - Blynk Version
 *
 * Alternative to Sinric Pro using Blynk IoT platform.
 * Blynk provides a mobile app for controlling your devices.
 *
 * Setup:
 * 1. Download Blynk app (iOS/Android)
 * 2. Create account at blynk.cloud
 * 3. Create new template with a Switch widget on V0
 * 4. Copy your credentials below
 *
 * @author osovlanski
 * @version 1.0.0
 * @license MIT
 */

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "SwitchBot"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ============================================================================
// CONFIGURATION
// ============================================================================
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// ============================================================================
// HARDWARE SETTINGS
// ============================================================================
#define SERVO_PIN     13
#define REST_ANGLE    90
#define PUSH_ANGLE    150
#define PULL_ANGLE    30
#define SERVO_DELAY   500

// ============================================================================
// BLYNK VIRTUAL PINS
// ============================================================================
#define VPIN_SWITCH   V0    // Main switch control
#define VPIN_PUSH     V1    // Manual push button
#define VPIN_PULL     V2    // Manual pull button
#define VPIN_STATUS   V3    // Status LED indicator

// ============================================================================
// GLOBALS
// ============================================================================
Servo switchServo;
BlynkTimer timer;
bool switchState = false;

// ============================================================================
// SWITCH ACTIONS
// ============================================================================

void pushSwitch() {
    Serial.println("[ACTION] Pushing switch...");
    switchServo.write(PUSH_ANGLE);
    delay(SERVO_DELAY);
    switchServo.write(REST_ANGLE);
    switchState = true;
    Blynk.virtualWrite(VPIN_STATUS, 255); // LED on
}

void pullSwitch() {
    Serial.println("[ACTION] Pulling switch...");
    switchServo.write(PULL_ANGLE);
    delay(SERVO_DELAY);
    switchServo.write(REST_ANGLE);
    switchState = false;
    Blynk.virtualWrite(VPIN_STATUS, 0); // LED off
}

// ============================================================================
// BLYNK CALLBACKS
// ============================================================================

// Main switch widget (V0)
BLYNK_WRITE(V0) {
    int value = param.asInt();
    Serial.printf("[BLYNK] Switch set to: %d\n", value);

    if (value == 1) {
        pushSwitch();
    } else {
        pullSwitch();
    }
}

// Push button (V1) - momentary push action
BLYNK_WRITE(V1) {
    if (param.asInt() == 1) {
        pushSwitch();
        Blynk.virtualWrite(VPIN_SWITCH, 1);
    }
}

// Pull button (V2) - momentary pull action
BLYNK_WRITE(V2) {
    if (param.asInt() == 1) {
        pullSwitch();
        Blynk.virtualWrite(VPIN_SWITCH, 0);
    }
}

// Called when Blynk connects
BLYNK_CONNECTED() {
    Serial.println("[BLYNK] Connected!");
    // Sync all virtual pins with server
    Blynk.syncAll();
}

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void checkConnection() {
    if (!Blynk.connected()) {
        Serial.println("[BLYNK] Connection lost, reconnecting...");
    }
}

// ============================================================================
// SETUP & LOOP
// ============================================================================

void setup() {
    Serial.begin(115200);
    Serial.println("\n========================================");
    Serial.println("   DIY SwitchBot (Blynk) - Starting");
    Serial.println("========================================\n");

    // Initialize servo
    switchServo.attach(SERVO_PIN);
    switchServo.write(REST_ANGLE);
    Serial.printf("[SERVO] Attached to GPIO %d\n", SERVO_PIN);

    // Connect to Blynk (also handles WiFi)
    Serial.println("[BLYNK] Connecting...");
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

    // Setup timer for connection checking
    timer.setInterval(30000L, checkConnection);

    Serial.println("\n[READY] SwitchBot is ready for commands!");
}

void loop() {
    Blynk.run();
    timer.run();
}




