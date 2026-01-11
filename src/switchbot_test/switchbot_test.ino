/**
 * DIY SwitchBot - TEST VERSION (For Wokwi Simulator)
 * 
 * This version demonstrates servo movement without WiFi.
 * Use this to test in Wokwi simulator before uploading to real hardware.
 * 
 * In Wokwi: The servo will move every 3 seconds to simulate button presses.
 * In Serial Monitor: Shows status messages.
 * 
 * @author osovlanski
 * @version 1.0.0
 */

#include <ESP32Servo.h>

// ============================================================================
// CONFIGURATION
// ============================================================================
#define SERVO_PIN     13
#define REST_ANGLE    90
#define PUSH_ANGLE    150
#define PULL_ANGLE    30
#define SERVO_DELAY   500

// ============================================================================
// GLOBALS
// ============================================================================
Servo switchServo;
bool switchState = false;
unsigned long lastToggleTime = 0;
const unsigned long TOGGLE_INTERVAL = 3000; // Toggle every 3 seconds for demo

// ============================================================================
// SWITCH ACTIONS
// ============================================================================

void pushSwitch() {
    Serial.println("[ACTION] Pushing switch → ON");
    switchServo.write(PUSH_ANGLE);
    delay(SERVO_DELAY);
    switchServo.write(REST_ANGLE);
    switchState = true;
    Serial.println("[STATUS] Switch is now ON");
}

void pullSwitch() {
    Serial.println("[ACTION] Pulling switch → OFF");
    switchServo.write(PULL_ANGLE);
    delay(SERVO_DELAY);
    switchServo.write(REST_ANGLE);
    switchState = false;
    Serial.println("[STATUS] Switch is now OFF");
}

void toggleSwitch() {
    if (switchState) {
        pullSwitch();
    } else {
        pushSwitch();
    }
}

// ============================================================================
// SETUP & LOOP
// ============================================================================

void setup() {
    Serial.begin(115200);
    delay(1000); // Wait for serial to initialize
    
    Serial.println("\n========================================");
    Serial.println("   DIY SwitchBot - TEST MODE");
    Serial.println("   (Wokwi Simulator Compatible)");
    Serial.println("========================================\n");
    
    // Initialize servo
    switchServo.attach(SERVO_PIN);
    switchServo.write(REST_ANGLE);
    Serial.printf("[SERVO] Attached to GPIO %d\n", SERVO_PIN);
    Serial.printf("[SERVO] Rest: %d°, Push: %d°, Pull: %d°\n", REST_ANGLE, PUSH_ANGLE, PULL_ANGLE);
    
    Serial.println("\n[INFO] Auto-toggling every 3 seconds for demo...");
    Serial.println("[INFO] Watch the servo move in the simulator!\n");
    
    // Initial push to show it's working
    delay(2000);
    pushSwitch();
}

void loop() {
    unsigned long currentTime = millis();
    
    // Auto-toggle for demonstration
    if (currentTime - lastToggleTime >= TOGGLE_INTERVAL) {
        lastToggleTime = currentTime;
        Serial.println("\n--- Auto Toggle ---");
        toggleSwitch();
    }
}

