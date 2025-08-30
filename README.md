# Smart Farm Defending System

## Overview

This project is a proof-of-concept **Smart Farm Defending System** designed to detect pest animals such as boars and coyotes and deter them using a non-lethal microcontroller-based turret system.

The system employs a **computer vision module** for detection and a **servo-controlled turret** for targeting and firing rubber-band pellets to scare intruding animals.

For demonstration purposes, the prototype detects human faces using Haar Cascade in Python, communicates target coordinates to an **Arduino Uno**, and controls turret servos to aim and fire. An **ESP32** was later integrated to add IoT functionality, including a **web dashboard** for remote monitoring and control.

---

## Features

* **Animal Detection (Proof of Concept):**

  * Haar Cascade classifier (demonstrated using human face detection).
* **Turret Control:**

  * Horizontal/vertical aiming with MG995 servos.
  * Rubber band firing mechanism via a trigger servo.
  * Laser aiming module.
* **Arduino Uno Control:**

  * Receives target coordinates from the computer vision module.
  * Controls turret orientation and firing logic.
* **ESP32 IoT Dashboard:**

  * Web interface accessible from smartphones/PCs.
  * Arm/Disarm gun control.
  * System Online/Offline toggle.
  * Real-time shot log (notifications when shots are fired).
* **Non-Lethal Deterrent:**
  Fires rubber bands as a safe, demonstrative substitute for pellets.

---

## System Architecture

1. **Computer (Python Haar Cascade Script)**

   * Detects intruders (faces for demo).
   * Sends target coordinates and commands via Serial to Arduino.
2. **Arduino Uno**

   * Controls turret servos (X, Y, trigger).
   * Manages laser and shot indication.
   * Communicates status to ESP32 through GPIO.
3. **ESP32**

   * Hosts a web dashboard.
   * Allows remote control (arm/disarm, system state).
   * Logs and displays shot events.

---

## Hardware Used

* Arduino Uno
* ESP32
* MG995 Servos (X-axis, Y-axis, Trigger)
* Laser module
* Rubber band gun (firing mechanism)
* GPIO-based communication between Arduino and ESP32
* Computer (for running Haar Cascade Python script)

---

## Software & Libraries

* **Python (OpenCV)** for Haar Cascade detection.
* **Arduino IDE** for firmware programming.
* **Arduino Libraries:**

  * `VarSpeedServo.h` (smooth servo control).
* **ESP32 Libraries:**

  * `WiFi.h`
  * `WebServer.h`

---

## Web Dashboard (ESP32)

* **System Toggle:** Online/Offline control.
* **Gun Toggle:** Arm/Disarm.
* **Notification Panel:** Logs shots with timestamps.
* **Dark Mode Interface:** Clean, mobile-friendly UI.

---

## Usage

1. **Setup Arduino & ESP32:**

   * Upload respective codes from this repo.
   * Ensure correct pin connections as per the circuit diagram.

2. **Configure ESP32 WiFi:**

   * Update `ssid` and `password` in `esp32_dashboard.ino`.

3. **Run Python Detection Script:**

   * Connect Arduino to PC.
   * Run the Haar Cascade script to detect faces and send target coordinates.

4. **Access Web Dashboard:**

   * Connect to ESP32 via its IP (printed in Serial Monitor).
   * Control gun arm/disarm and view shot logs remotely.

---

## Demonstration Flow

1. Python detects a face in frame.
2. Coordinates sent to Arduino.
3. Arduino orients turret using servos.
4. Laser aligns, rubber band gun fires after safety interval.
5. Arduino signals shot fired to ESP32 via GPIO.
6. ESP32 logs and displays event on web dashboard.

---

## Limitations & Future Work

* Current prototype detects **faces** (for demo) instead of animals.
* Vision processing runs on PC instead of embedded hardware.
* System communication between Arduino and ESP32 via GPIO (basic method).
* Future scope:

  * Replace Haar Cascade with trained animal detection models.
  * Integrate detection and turret control on a single embedded system.
  * Enhance IoT features with cloud-based logging.

---

## Disclaimer

This project is a **proof of concept** intended for academic and hobbyist demonstration only.
The system is designed as a **non-lethal deterrent** using rubber bands for testing purposes.

