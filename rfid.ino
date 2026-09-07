#include <WiFi.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// =================================================
// Wi-Fi
// =================================================

const char* ssid = "Y16";
const char* password = "deekshith";

WiFiServer server(80);

// =================================================
// MOTOR PINS
// =================================================

#define ENA 25
#define IN1 26
#define IN2 27

#define ENB 33
#define IN3 14
#define IN4 12

// =================================================
// ULTRASONIC
// =================================================

#define TRIG_PIN 5
#define ECHO_PIN 18

// =================================================
// SETTINGS
// =================================================

int speedValue = 180;

const int OBSTACLE_DISTANCE = 25;

bool autoMode = false;

// =================================================
// PWM
// =================================================

#define PWM_FREQ 1000
#define PWM_RES 8

#define CHANNEL_A 0
#define CHANNEL_B 1;


// =================================================
// SETUP
// =================================================

void setup() {

  Serial.begin(115200);

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // PWM
  ledcSetup(CHANNEL_A, PWM_FREQ, PWM_RES);
  ledcSetup(CHANNEL_B, PWM_FREQ, PWM_RES);

  ledcAttachPin(ENA, CHANNEL_A);
  ledcAttachPin(ENB, CHANNEL_B);

  stopRobot();

  // =================================================
  // Wi-Fi Access Point
  // =================================================

  WiFi.mode(WIFI_AP);

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("================================");
  Serial.println("     ESP32 ROBOT");
  Serial.println("================================");

  Serial.print("Wi-Fi IP: ");
  Serial.println(WiFi.softAPIP());

  server.begin();

  // =================================================
  // Bluetooth
  // =================================================

  if (SerialBT.begin("ESP32_ROBOT")) {

    Serial.println("Bluetooth started");
    Serial.println("Device: ESP32_ROBOT");

  } else {

    Serial.println("Bluetooth failed");

  }

  Serial.println("================================");
}


// =================================================
// MAIN LOOP
// =================================================

void loop() {

  // Check Bluetooth
  bluetoothControl();

  // Check Wi-Fi
  wifiControl();

  // Automatic mode
  if (autoMode) {

    automaticMode();

  }

  delay(20);
}


// =================================================
// BLUETOOTH CONTROL
// =================================================

void bluetoothControl() {

  if (SerialBT.available()) {

    char command = SerialBT.read();

    Serial.print("BT Command: ");
    Serial.println(command);

    processCommand(command);
  }
}


// =================================================
// COMMAND PROCESSING
// =================================================

void processCommand(char command) {

  switch (command) {

    // -----------------------------
    // MANUAL
    // -----------------------------

    case 'F':
    case 'f':

      autoMode = false;
      forward();

      break;


    case 'B':
    case 'b':

      autoMode = false;
      backward();

      break;


    case 'L':
    case 'l':

      autoMode = false;
      turnLeft();

      break;


    case 'R':
    case 'r':

      autoMode = false;
      turnRight();

      break;


    case 'S':
    case 's':

      autoMode = false;
      stopRobot();

      break;


    // -----------------------------
    // AUTO MODE
    // -----------------------------

    case 'A':
    case 'a':

      autoMode = true;

      stopRobot();

      Serial.println("AUTO MODE");

      break;


    // -----------------------------
    // MANUAL MODE
    // -----------------------------

    case 'M':
    case 'm':

      autoMode = false;

      stopRobot();

      Serial.println("MANUAL MODE");

      break;


    // -----------------------------
    // SPEED
    // -----------------------------

    case '+':

      speedValue += 20;

      if (speedValue > 255)
        speedValue = 255;

      Serial.print("Speed: ");
      Serial.println(speedValue);

      break;


    case '-':

      speedValue -= 20;

      if (speedValue < 80)
        speedValue = 80;

      Serial.print("Speed: ");
      Serial.println(speedValue);

      break;
  }
}


// =================================================
// AUTOMATIC MODE
// =================================================

void automaticMode() {

  long distance = getDistance();

  Serial.print("AUTO Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  // Invalid ultrasonic reading
  if (distance == 0) {

    stopRobot();

    return;
  }


  // Path clear

  if (distance > OBSTACLE_DISTANCE) {

    forward();

  }


  // Obstacle detected

  else {

    Serial.println("OBSTACLE!");

    stopRobot();

    delay(200);

    backward();

    delay(350);

    stopRobot();

    delay(200);

    // Randomly choose direction
    // This prevents getting stuck

    if (millis() % 2 == 0) {

      turnLeft();

      delay(550);

    } else {

      turnRight();

      delay(550);

    }

    stopRobot();

    delay(100);
  }
}


// =================================================
// ULTRASONIC
// =================================================

long getDistance() {

  digitalWrite(TRIG_PIN, LOW);

  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);


  long duration =
    pulseIn(ECHO_PIN, HIGH, 30000);


  if (duration == 0) {

    return 0;

  }


  long distance =
    duration * 0.0343 / 2;


  return distance;
}


// =================================================
// FORWARD
// =================================================

void forward() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(CHANNEL_A, speedValue);
  ledcWrite(CHANNEL_B, speedValue);
}


// =================================================
// BACKWARD
// =================================================

void backward() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(CHANNEL_A, speedValue);
  ledcWrite(CHANNEL_B, speedValue);
}


// =================================================
// LEFT
// =================================================

void turnLeft() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(CHANNEL_A, speedValue);
  ledcWrite(CHANNEL_B, speedValue);
}


// =================================================
// RIGHT
// =================================================

void turnRight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(CHANNEL_A, speedValue);
  ledcWrite(CHANNEL_B, speedValue);
}


// =================================================
// STOP
// =================================================

void stopRobot() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  ledcWrite(CHANNEL_A, 0);
  ledcWrite(CHANNEL_B, 0);
}


// =================================================
// WI-FI WEB CONTROL
// =================================================

void wifiControl() {

  WiFiClient client = server.available();

  if (!client)
    return;


  String request = "";

  unsigned long timeout = millis();


  while (client.connected() &&
         millis() - timeout < 1000) {

    if (client.available()) {

      char c = client.read();

      request += c;

      if (c == '\n') {

        break;
      }
    }
  }


  // Commands from webpage

  if (request.indexOf("/F") != -1) {

    autoMode = false;
    forward();

  }

  else if (request.indexOf("/B") != -1) {

    autoMode = false;
    backward();

  }

  else if (request.indexOf("/L") != -1) {

    autoMode = false;
    turnLeft();

  }

  else if (request.indexOf("/R") != -1) {

    autoMode = false;
    turnRight();

  }

  else if (request.indexOf("/S") != -1) {

    autoMode = false;
    stopRobot();

  }

  else if (request.indexOf("/AUTO") != -1) {

    autoMode = true;
    stopRobot();

  }

  else if (request.indexOf("/MANUAL") != -1) {

    autoMode = false;
    stopRobot();

  }


  // Web page

  client.println(
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n"
  );


  client.println(
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>ESP32 Robot</title>"
    "<style>"
    "body{text-align:center;font-family:Arial;background:#111;color:white}"
    "button{width:120px;height:65px;margin:8px;font-size:20px;border-radius:15px}"
    ".auto{width:260px}"
    "</style>"
    "</head>"
    "<body>"
  );


  client.println("<h1>🤖 ESP32 ROBOT</h1>");


  client.println(
    "<p>Wi-Fi Robot Control</p>"
  );


  // Forward

  client.println(
    "<a href='/F'><button>⬆️<br>FORWARD</button></a>"
  );


  client.println("<br>");


  // Left

  client.println(
    "<a href='/L'><button>⬅️ LEFT</button></a>"
  );


  // Stop

  client.println(
    "<a href='/S'><button>🛑 STOP</button></a>"
  );


  // Right

  client.println(
    "<a href='/R'><button>RIGHT ➡️</button></a>"
  );


  client.println("<br>");


  // Backward

  client.println(
    "<a href='/B'><button>⬇️<br>BACK</button></a>"
  );


  client.println("<br><br>");


  // Auto

  client.println(
    "<a href='/AUTO'>"
    "<button class='auto'>🤖 AUTO MODE</button>"
    "</a>"
  );


  client.println("<br>");


  // Manual

  client.println(
    "<a href='/MANUAL'>"
    "<button class='auto'>🎮 MANUAL MODE</button>"
    "</a>"
  );


  client.println(
    "</body></html>"
  );


  delay(1);

  client.stop();
}