/**
 * DIY SwitchBot - Simple Version (No Cloud)
 * 
 * This is a simplified version that creates a local web server
 * to control the switch. No cloud account needed!
 * 
 * Access the switch via: http://<ESP32_IP_ADDRESS>/
 * 
 * Hardware:
 *   - ESP32 Dev Board
 *   - SG90 Micro Servo
 * 
 * @author osovlanski
 * @version 1.0.0
 * @license MIT
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ============================================================================
// CONFIGURATION - UPDATE THESE!
// ============================================================================
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";

// ============================================================================
// PIN & SERVO SETTINGS
// ============================================================================
#define SERVO_PIN     13
#define REST_ANGLE    90
#define PUSH_ANGLE    150
#define PULL_ANGLE    30
#define SERVO_DELAY   500

// ============================================================================
// GLOBAL OBJECTS
// ============================================================================
Servo switchServo;
WebServer server(80);
bool switchState = false;

// ============================================================================
// WEB PAGE HTML
// ============================================================================
const char* HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>DIY SwitchBot</title>
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: linear-gradient(135deg, #1a1a2e 0%, #16213e 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        .container {
            background: rgba(255, 255, 255, 0.05);
            backdrop-filter: blur(10px);
            border-radius: 24px;
            padding: 40px;
            text-align: center;
            border: 1px solid rgba(255, 255, 255, 0.1);
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
            max-width: 400px;
            width: 100%;
        }
        h1 {
            color: #e94560;
            font-size: 2rem;
            margin-bottom: 10px;
            letter-spacing: -0.5px;
        }
        .subtitle {
            color: rgba(255, 255, 255, 0.6);
            font-size: 0.9rem;
            margin-bottom: 30px;
        }
        .status {
            font-size: 1.1rem;
            color: #fff;
            margin-bottom: 30px;
            padding: 12px 24px;
            background: rgba(255, 255, 255, 0.05);
            border-radius: 12px;
            display: inline-block;
        }
        .status-indicator {
            display: inline-block;
            width: 12px;
            height: 12px;
            border-radius: 50%;
            margin-right: 10px;
            animation: pulse 2s infinite;
        }
        .status-on { background: #00d26a; box-shadow: 0 0 20px #00d26a; }
        .status-off { background: #ff6b6b; box-shadow: 0 0 20px #ff6b6b; }
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        .btn-group { display: flex; gap: 12px; justify-content: center; flex-wrap: wrap; }
        .btn {
            padding: 16px 32px;
            font-size: 1rem;
            font-weight: 600;
            border: none;
            border-radius: 12px;
            cursor: pointer;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        .btn:active { transform: scale(0.95); }
        .btn-on {
            background: linear-gradient(135deg, #00d26a, #00a854);
            color: white;
        }
        .btn-on:hover { box-shadow: 0 10px 30px rgba(0, 210, 106, 0.4); }
        .btn-off {
            background: linear-gradient(135deg, #ff6b6b, #ee5a5a);
            color: white;
        }
        .btn-off:hover { box-shadow: 0 10px 30px rgba(255, 107, 107, 0.4); }
        .btn-toggle {
            background: linear-gradient(135deg, #667eea, #764ba2);
            color: white;
            width: 100%;
            margin-top: 12px;
        }
        .btn-toggle:hover { box-shadow: 0 10px 30px rgba(102, 126, 234, 0.4); }
    </style>
</head>
<body>
    <div class="container">
        <h1>🤖 SwitchBot</h1>
        <p class="subtitle">DIY Smart Switch Controller</p>
        <div class="status">
            <span class="status-indicator %STATUS_CLASS%"></span>
            Switch is <strong>%STATUS%</strong>
        </div>
        <div class="btn-group">
            <button class="btn btn-on" onclick="fetch('/on').then(()=>location.reload())">ON</button>
            <button class="btn btn-off" onclick="fetch('/off').then(()=>location.reload())">OFF</button>
        </div>
        <button class="btn btn-toggle" onclick="fetch('/toggle').then(()=>location.reload())">Toggle</button>
    </div>
</body>
</html>
)rawliteral";

// ============================================================================
// SWITCH ACTIONS
// ============================================================================

void pushSwitch() {
    Serial.println("[ACTION] Pushing switch...");
    switchServo.write(PUSH_ANGLE);
    delay(SERVO_DELAY);
    switchServo.write(REST_ANGLE);
    switchState = true;
}

void pullSwitch() {
    Serial.println("[ACTION] Pulling switch...");
    switchServo.write(PULL_ANGLE);
    delay(SERVO_DELAY);
    switchServo.write(REST_ANGLE);
    switchState = false;
}

// ============================================================================
// WEB SERVER HANDLERS
// ============================================================================

void handleRoot() {
    String html = String(HTML_PAGE);
    html.replace("%STATUS%", switchState ? "ON" : "OFF");
    html.replace("%STATUS_CLASS%", switchState ? "status-on" : "status-off");
    server.send(200, "text/html", html);
}

void handleOn() {
    pushSwitch();
    server.send(200, "text/plain", "Switch turned ON");
}

void handleOff() {
    pullSwitch();
    server.send(200, "text/plain", "Switch turned OFF");
}

void handleToggle() {
    if (switchState) {
        pullSwitch();
    } else {
        pushSwitch();
    }
    server.send(200, "text/plain", switchState ? "Switch is ON" : "Switch is OFF");
}

void handleStatus() {
    server.send(200, "application/json", 
        "{\"state\":\"" + String(switchState ? "on" : "off") + "\"}");
}

// ============================================================================
// SETUP & LOOP
// ============================================================================

void setup() {
    Serial.begin(115200);
    Serial.println("\n========================================");
    Serial.println("   DIY SwitchBot (Simple) - Starting");
    Serial.println("========================================\n");
    
    // Initialize servo
    switchServo.attach(SERVO_PIN);
    switchServo.write(REST_ANGLE);
    Serial.printf("[SERVO] Attached to GPIO %d\n", SERVO_PIN);
    
    // Connect to WiFi
    Serial.printf("[WIFI] Connecting to %s", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[WIFI] Connected!");
    Serial.printf("[WIFI] IP Address: %s\n", WiFi.localIP().toString().c_str());
    
    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.on("/toggle", handleToggle);
    server.on("/status", handleStatus);
    
    server.begin();
    Serial.println("\n[READY] Web server started!");
    Serial.printf("[READY] Open http://%s/ in your browser\n", WiFi.localIP().toString().c_str());
}

void loop() {
    server.handleClient();
    
    // Reconnect if WiFi drops
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[WIFI] Reconnecting...");
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
        }
    }
}

