#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Your WiFi SSID";
const char* password = "Your WiFi Password";

const int gunpin = 12;
const int system_ctrl = 14;
const int shot_indicator = 13;
// Create WebServer object on port 80
WebServer server(80);

int systemOnline = 1;  // System is online by default
int gunArmed = 0;      // Gun is disarmed by default

// Dark mode HTML content with pill-shaped buttons, styled message box, and scrolling notifications
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Smart Farm Defender</title>
  <style>
    body {
      font-family: 'Arial', sans-serif;
      background-color: #121212;  /* Dark background */
      color: #e0e0e0;             /* Light text */
    }
    h1 {
      background-color: #1F1F1F;
      color: #ffffff;
      padding: 15px;
      text-align: center;
      border-radius: 5px;
      font-size: 24px;
    }
    .container {
      width: 90%;
      margin: 20px auto;
    }
    .notification-box {
      background-color: #2c2c2c;   /* Slightly lighter background */
      padding: 15px;
      height: 200px;
      overflow-y: auto;
      border: 2px solid #444;      /* Dark border */
      font-size: 18px;
      color: #ffffff;              /* Light text for notifications */
    }
    .notification-box h2 {
      font-size: 18px;
      margin-top: 0;
      margin-bottom: 10px;
      color: #bdbdbd;
    }
    .button-container {
      display: flex;
      justify-content: space-between;
      margin-top: 20px;
    }
    .btn {
      padding: 20px 40px;
      font-size: 18px;
      color: white;
      background-color: #333333;   /* Default button background in dark mode */
      border: none;
      border-radius: 30px;         /* Pill-shaped buttons */
      cursor: pointer;
      flex-grow: 1;
      margin: 0 15px;
      transition: background-color 0.3s ease;
    }
    .btn:hover {
      background-color: #444444;   /* Slight hover effect */
    }
    .btn.toggle-on {
      background-color: #388E3C;   /* Green for system online */
    }
    .btn.toggle-off {
      background-color: #D32F2F;   /* Red for system offline */
    }
  </style>
</head>
<body>
  <h1>Smart Farm Defender</h1>
  <div class="container">
    <div class="notification-box" id="notification-box">
      <h2>Notification</h2>
    </div>

    <div class="button-container">
      <button class="btn toggle-on" id="system-btn" onclick="toggleSystem()">System Online</button>
      <button class="btn" id="gun-btn" onclick="toggleGun()">Gun Arm</button>
    </div>
  </div>

  <script>
    let systemOnline = true;
    let gunArmed = false;

    function toggleSystem() {
      systemOnline = !systemOnline;
      document.getElementById('system-btn').classList.toggle('toggle-on', systemOnline);
      document.getElementById('system-btn').classList.toggle('toggle-off', !systemOnline);
      document.getElementById('system-btn').innerHTML = systemOnline ? 'System Online' : 'System Offline';
      fetch('/system?status=' + (systemOnline ? 'online' : 'offline'));
    }

    function toggleGun() {
      gunArmed = !gunArmed;
      document.getElementById('gun-btn').innerHTML = gunArmed ? 'Disarm Gun' : 'Gun Arm';
      fetch('/gun?status=' + (gunArmed ? 'armed' : 'disarmed'));
    }

    // Function to display notifications with timestamp
    function addNotification(message) {
      const notificationBox = document.getElementById('notification-box');
      const timestamp = new Date().toLocaleTimeString();
      notificationBox.innerHTML += `<p>[${timestamp}] ${message}</p>`;
      notificationBox.scrollTop = notificationBox.scrollHeight;
    }

    // Poll for pin shot_indicator state
    setInterval(() => {
      fetch('/check-shot')
        .then(response => response.text())
        .then(data => {
          if (data === 'shot-fired') {
            addNotification('Shot Fired');
          }
        });
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

// Function prototypes
void handleRoot();
void handleSystem();
void handleGun();
void checkShot();

void setup() {
  Serial.begin(115200);

  // Initialize pins
  pinMode(system_ctrl, OUTPUT);  // System Online/Offline pin
  pinMode(gunpin, OUTPUT);  // Gun Arm/Disarm pin
  pinMode(shot_indicator, INPUT);  // Pin for Shot Fired

  digitalWrite(4, HIGH);  // System is online by default

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Serve the HTML page
  server.on("/", HTTP_GET, handleRoot);

  // Handle system toggle
  server.on("/system", HTTP_GET, handleSystem);

  // Handle gun toggle
  server.on("/gun", HTTP_GET, handleGun);

  // Check shot fired
  server.on("/check-shot", HTTP_GET, checkShot);

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleSystem() {
  String status = server.arg("status");
  if (status == "online") {
    digitalWrite(system_ctrl, HIGH);
    Serial.println("System Online");
  } else {
    digitalWrite(system_ctrl, LOW);
    Serial.println("System Offline");
  }
  server.send(200, "text/plain", "OK");
}

void handleGun() {
  String status = server.arg("status");
  if (status == "armed") {
    digitalWrite(gunpin, HIGH);
    Serial.println("Gun Armed");
  } else {
    digitalWrite(gunpin, LOW);
    Serial.println("Gun Disarmed");
  }
  server.send(200, "text/plain", "OK");
}

void checkShot() {
  if (digitalRead(shot_indicator) == HIGH) {
    server.send(200, "text/plain", "shot-fired");
  } else {
    server.send(200, "text/plain", "no-shot");
  }
}
