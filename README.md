🔐 ESP32 RFID Smart Door Lock
📌 Project Overview

The ESP32 RFID Smart Door Lock is an embedded security system that provides keyless door access using RFID authentication.

An authorized RFID card is detected by the RC522 RFID reader and verified by the ESP32. If the card is authorized, a servo motor unlocks the door for 5 seconds and then automatically locks it again.

The system also includes an OLED display for user feedback and a buzzer-based security alarm that activates after three consecutive unauthorized RFID attempts.

✨ Features
🪪 RFID-based authentication
🔐 Automatic door locking/unlocking
⚙️ Servo motor door control
🖥️ 0.96" OLED status display
🔊 Buzzer notification
🚨 Security alarm after 3 failed attempts
🔄 Automatic door relocking
⚡ ESP32-based embedded system
🔒 Keyless access control
🧰 Components Required
Component	Quantity
ESP32 DevKit V1	1
RC522 RFID Module	1
RFID Card/Tag	1+
SG90 Servo Motor	1
0.96" SSD1306 OLED	1
Buzzer	1
Jumper Wires	As required
5V Power Supply	1
🔌 Pin Configuration
RC522 → ESP32
RC522	ESP32
SDA / SS	GPIO 5
SCK	GPIO 18
MOSI	GPIO 23
MISO	GPIO 19
RST	GPIO 22
3.3V	3V3
GND	GND
OLED → ESP32
OLED	ESP32
SDA	GPIO 21
SCL	GPIO 26
VCC	3V3
GND	GND
Servo → ESP32
Servo	ESP32
Signal (Yellow/Orange)	GPIO 13
VCC (Red)	5V/VIN
GND (Brown/Black)	GND
Buzzer → ESP32
Buzzer	ESP32
+	GPIO 27
−	GND

⚠️ Important: The RC522 and OLED use 3.3V. Do not power the RC522 from 5V. For reliable operation, power the servo from a suitable 5V supply and keep the servo/ESP32 grounds common.

🧠 Block Diagram
                 ┌─────────────────┐
                 │   RFID CARD     │
                 └────────┬────────┘
                          │
                          ▼
                 ┌─────────────────┐
                 │   RC522 RFID    │
                 │     READER      │
                 └────────┬────────┘
                          │ SPI
                          ▼
                 ┌─────────────────┐
                 │      ESP32      │
                 │  AUTHENTICATION │
                 └───┬────┬────┬───┘
                     │    │    │
                     ▼    ▼    ▼
                  OLED  BUZZER SERVO
                   │      │      │
                   ▼      ▼      ▼
                STATUS   ALARM  DOOR
                              LOCK
⚙️ Working Principle
The system starts with the door in the locked position.
OLED displays:
SMART
DOOR LOCK

SCAN RFID CARD
User places an RFID card near the RC522.
RC522 reads the card UID.
ESP32 compares the UID with the stored authorized UID.
If the UID matches:
OLED displays ACCESS GRANTED
Buzzer gives a confirmation beep
Servo moves to the unlock position
Door remains unlocked for 5 seconds
Servo returns to the locked position
If the UID does not match:
OLED displays ACCESS DENIED
Buzzer gives warning beeps
Failed-attempt counter increases
After 3 unauthorized attempts:
OLED displays SECURITY ALARM
Buzzer sounds for approximately 5 seconds
Failed-attempt counter resets.
🔑 Authorized RFID Card

The currently programmed authorized card UID is:

D1:70:8E:68

To use another card, update the authorizedUID[] array in the Arduino code.

📊 System Logic
START
  │
  ▼
Initialize ESP32
  │
  ├── RC522
  ├── OLED
  ├── Servo
  └── Buzzer
  │
  ▼
Door LOCKED
  │
  ▼
Scan RFID
  │
  ▼
Read UID
  │
  ▼
Is UID authorized?
  │
 ┌┴───────────────┐
 │                │
YES              NO
 │                │
 ▼                ▼
ACCESS          ACCESS
GRANTED         DENIED
 │                │
 ▼                ▼
UNLOCK          Attempt++
 │                │
 ▼                ▼
5 Seconds       3 Attempts?
 │                │
 ▼              YES
LOCK              │
 │                ▼
 ▼             ALARM
SCAN AGAIN        │
                  ▼
              Reset Counter
💻 Software
Development Environment
Arduino IDE
ESP32 Board Package
Libraries

Install these libraries from Arduino IDE Library Manager:

MFRC522
ESP32Servo
Adafruit GFX Library
Adafruit SSD1306
📁 Recommended GitHub Repository Structure
ESP32-RFID-Smart-Door-Lock/
│
├── README.md
│
├── src/
│   └── smart_door_lock.ino
│
├── circuit/
│   └── circuit_diagram.png
│
├── images/
│   ├── project.jpg
│   ├── oled.jpg
│   └── wiring.jpg
│
├── docs/
│   └── project_report.pdf
│
└── LICENSE
🚀 How to Run
1. Install Arduino IDE

Download and install Arduino IDE.

2. Add ESP32 Board

Install the ESP32 board package through Arduino IDE's Boards Manager.

3. Install libraries

Install:

MFRC522
ESP32Servo
Adafruit GFX
Adafruit SSD1306
4. Connect the hardware

Follow the pin configuration above.

5. Open the code
src/smart_door_lock.ino
6. Select the board
ESP32 Dev Module

Select the correct COM port.

7. Upload

Upload the program to the ESP32.

8. Test

Scan the authorized RFID card:

D1:70:8E:68

The servo should unlock and automatically lock after 5 seconds.

🧪 Test Results
Test	Expected Result	Status
ESP32 startup	System initializes	✅
RFID detection	UID displayed	✅
Authorized card	Door unlocks	✅
Unauthorized card	Access denied	✅
3 wrong cards	Security alarm	✅
OLED	Status displayed	✅
Servo	Lock/unlock operation	✅
Buzzer	Warning/alarm	✅
🔮 Future Improvements

Possible future upgrades:

Fingerprint authentication
Keypad PIN authentication
Mobile application control
Wi-Fi notifications
GSM/SMS security alerts
Magnetic door sensor
Cloud access logs
Multiple authorized RFID cards
Real-time access history
🎯 Applications

This system can be used for:

🏠 Smart homes
🏢 Offices
🏫 Laboratories
🔬 College projects
🏭 Restricted industrial areas
🗄️ Secure cabinets
🚪 Electronic access-control systems
👨‍💻 Skills Demonstrated

This project demonstrates practical experience in:

Embedded C/C++
ESP32 programming
SPI communication
I²C communication
RFID authentication
Servo motor control
OLED interfacing
Digital I/O
Security-system design
Hardware interfacing
Arduino IDE
