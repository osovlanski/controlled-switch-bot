/**
 * DIY SwitchBot - ESP32 Based Smart Switch Controller
 *
 * This project creates a WiFi-enabled switch controller that can be
 * controlled via Alexa, Google Home, or any Sinric Pro compatible app.
 *
 * Hardware:
 *   - ESP32 Dev Board
 *   - SG90 Micro Servo
 *   - 3.7V LiPo Battery (optional, can use USB power)
 *
 * Wiring:
 *   - Servo Red (VCC)    -> ESP32 VIN (or 5V)
 *   - Servo Brown (GND)  -> ESP32 GND
 *   - Servo Orange (PWM) -> ESP32 GPIO 13
 *
 * @author osovlanski
 * @version 1.0.0
 * @license MIT
 */

#include <WiFi.h>
#include <ESP32Servo.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

#include "config.h"

// ============================================================================
// PIN DEFINITIONS
// ============================================================================
#define SERVO_PIN         13    // GPIO pin for servo signal (Orange wire)

// ============================================================================
// SERVO POSITION SETTINGS
// ============================================================================
#define REST_ANGLE        90    // Middle/idle position (degrees)
#define PUSH_ANGLE        150   // Position when pushing the button (degrees)
#define PULL_ANGLE        30    // Position when pulling the switch (degrees)
#define SERVO_DELAY_MS    500   // Time to hold the push/pull position (ms)

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================
Servo switchServo;
bool currentPowerState = false;

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

/**
 * Moves servo to push the switch, then returns to rest position
 */
void pushSwitch() {
    Serial.println("[ACTION] Pushing switch...");
    switchServo.write(PUSH_ANGLE);
    delay(SERVO_DELAY_MS);
    switchServo.write(REST_ANGLE);
    Serial.println("[ACTION] Push complete, returned to rest");
}

/**
 * Moves servo to pull the switch, then returns to rest position
 * (Useful for rocker switches that need to be pulled back)
 */
void pullSwitch() {
    Serial.println("[ACTION] Pulling switch...");
    switchServo.write(PULL_ANGLE);
    delay(SERVO_DELAY_MS);
    switchServo.write(REST_ANGLE);
    Serial.println("[ACTION] Pull complete, returned to rest");
}

/**
 * Connects to WiFi network with retry logic
 */
void connectToWiFi() {
    Serial.printf("\n[WIFI] Connecting to %s", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    int retryCount = 0;
    const int maxRetries = 30;

    while (WiFi.status() != WL_CONNECTED && retryCount < maxRetries) {
        delay(500);
        Serial.print(".");
        retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[WIFI] Connected!");
        Serial.printf("[WIFI] IP Address: %s\n", WiFi.localIP().toString().c_str());
    } else {
        Serial.println("\n[WIFI] Connection failed! Restarting...");
        delay(1000);
        ESP.restart();
    }
}

// ============================================================================
// SINRIC PRO CALLBACKS
// ============================================================================

/**
 * Callback function for power state changes from Sinric Pro
 * This is triggered when you use voice commands or the app
 *
 * @param deviceId The Sinric Pro device ID
 * @param state The requested power state (ON/OFF)
 * @return true if the action was successful
 */
bool onPowerState(const String &deviceId, bool &state) {
    Serial.printf("[SINRIC] Device %s turned %s\n", deviceId.c_str(), state ? "ON" : "OFF");

    currentPowerState = state;

    if (state) {
        pushSwitch();  // Turn ON = push
    } else {
        pullSwitch();  // Turn OFF = pull (for rocker switches)
    }

    return true;
}

/**
 * Initializes and configures Sinric Pro connection
 */
void setupSinricPro() {
    Serial.println("[SINRIC] Setting up Sinric Pro...");

    SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
    mySwitch.onPowerState(onPowerState);

    SinricPro.onConnected([]() {
        Serial.println("[SINRIC] Connected to Sinric Pro!");
    });

    SinricPro.onDisconnected([]() {
        Serial.println("[SINRIC] Disconnected from Sinric Pro!");
    });

    SinricPro.begin(APP_KEY, APP_SECRET);
    Serial.println("[SINRIC] Setup complete!");
}

// ============================================================================
// MAIN SETUP & LOOP
// ============================================================================

void setup() {
    // Initialize serial for debugging
    Serial.begin(115200);
    Serial.println("\n========================================");
    Serial.println("   DIY SwitchBot - Starting Up...");
    Serial.println("========================================\n");

    // Initialize servo
    Serial.println("[SERVO] Initializing servo...");
    switchServo.attach(SERVO_PIN);
    switchServo.write(REST_ANGLE);
    Serial.printf("[SERVO] Attached to GPIO %d, initial angle: %d\n", SERVO_PIN, REST_ANGLE);

    // Connect to WiFi
    connectToWiFi();

    // Setup Sinric Pro
    setupSinricPro();

    Serial.println("\n[READY] SwitchBot is ready for commands!\n");
}

void loop() {
    // Handle Sinric Pro events
    SinricPro.handle();

    // Reconnect WiFi if disconnected
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WIFI] Connection lost! Reconnecting...");
        connectToWiFi();
    }
}




