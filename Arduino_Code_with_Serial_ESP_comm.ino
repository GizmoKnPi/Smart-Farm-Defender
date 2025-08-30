#include <VarSpeedServo.h>

VarSpeedServo servoX;  // Servo for horizontal movement
VarSpeedServo servoY;  // Servo for vertical movement
VarSpeedServo triggerServo;  // Servo for trigger sweep (0 to 90 degrees)

// Define pins for laser, shot fired indication, and system control
const int laserPin = 2;            // Pin for laser control
const int shotFiredPin = 3;        // Pin for shot fired indication
const int systemControlPin = 5;    // Pin to control system operation

int lastAngleX = 90;  // Track the last servo angle to prevent unnecessary movement
int lastAngleY = 90;

unsigned long shotStartTime = 0;        // Track the time when the shot was fired
const unsigned long shotDuration = 500; // Duration of shot (in milliseconds)
bool shotInProgress = false;            // Flag to track if a shot is in progress

unsigned long lastShotTime = 0;         // Track the last time a shot was fired
const unsigned long shotInterval = 5000; // 5-second interval between shots

bool shotAllowed = true;                // Flag to track if shooting is allowed

void setup() {
  Serial.begin(9600);
  
  // Attach servos to their pins
  servoX.attach(9);  // Connect servo X to pin 9
  servoY.attach(10); // Connect servo Y to pin 10
  triggerServo.attach(6); // Connect trigger servo to pin 6

  // Set up laser, shot fired, and system control pins as output/input
  pinMode(laserPin, OUTPUT);
  pinMode(shotFiredPin, OUTPUT);
  pinMode(systemControlPin, INPUT_PULLUP);  // Pull-up to read HIGH/LOW state

  // Ensure all outputs start LOW
  digitalWrite(laserPin, LOW);
  digitalWrite(shotFiredPin, LOW);

  // Initialize servos to a starting position
  servoX.write(90, 150);  // Center position with max speed
  servoY.write(90, 150);  // Center position with max speed
  triggerServo.write(0);  // Trigger servo starts at 0 degrees
}

void loop() {
  // Check the system control pin state
  if (digitalRead(systemControlPin) == LOW) {
    // System control is LOW, reset servos to default and disable actions
    servoX.write(90, 150);  // Center position
    servoY.write(90, 150);  // Center position
    digitalWrite(laserPin, LOW);         // Laser OFF
    digitalWrite(shotFiredPin, LOW);     // Shot fired OFF
    triggerServo.write(0);               // Reset trigger servo to 0 degrees
    return;  // Skip the rest of the loop when system control is LOW
  }

  if (Serial.available()) {
    // Read the incoming data
    String input = Serial.readStringUntil('\n');
    int commaIndex = input.indexOf(',');

    // Process servo angles if coordinates received
    if (commaIndex > 0) {
      int angleX = input.substring(0, commaIndex).toInt();
      int angleY = input.substring(commaIndex + 1).toInt();

      // Limit angles to the servo range
      angleX = constrain(angleX, 35, 145);
      angleY = constrain(angleY, 0, 120);

      // Only move servos if the change is significant (to avoid shaking)
      if (abs(angleX - lastAngleX) > 5) {
        servoX.write(180 - angleX, 200); // Use VarSpeedServo for smooth movement
        lastAngleX = angleX;
      }
      if (abs(angleY - lastAngleY) > 5) {
        servoY.write(angleY, 200); // Use VarSpeedServo for smooth movement
        lastAngleY = angleY;
      }
    }

    // Check for shooting command and if enough time has passed since the last shot
    if (input == "shoot" && shotAllowed && !shotInProgress) {
      // Trigger laser and indicate shot fired
      digitalWrite(laserPin, HIGH);      // Laser ON
      digitalWrite(shotFiredPin, HIGH);  // Shot fired ON

      // Record the start time of the shot
      shotStartTime = millis();
      shotInProgress = true;

      // Sweep the trigger servo from 0 to 90 degrees in 1.5 seconds
      triggerServo.write(90, 200);   // Move trigger servo to 90 degrees with a smooth speed

      // Prevent further shots for the interval duration
      shotAllowed = false;
      lastShotTime = millis();           // Record time of this shot
    }
  }

  // Check if the shot duration has passed
  if (shotInProgress && (millis() - shotStartTime >= shotDuration + 1000)) {
    // Turn off laser and reset shot fired pin
    digitalWrite(laserPin, LOW);       // Laser OFF
    digitalWrite(shotFiredPin, LOW);   // Shot fired OFF
    shotInProgress = false;

    // Return the trigger servo to the 0-degree position
    triggerServo.write(0, 200);  // Sweep back to 0 degrees with a smooth speed
  }

  // Allow shooting again after 5 seconds have passed
  if (!shotAllowed && (millis() - lastShotTime >= shotInterval)) {
    shotAllowed = true;  // Re-enable shooting after interval
  }
}
