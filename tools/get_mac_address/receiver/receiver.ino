#include <esp_now.h>
#include <WiFi.h>

// --- L298N #1 Pin Definitions (Front Motors) ---
#define M1_ENA  25
#define M1_IN1  26
#define M1_IN2  27
#define M1_IN3  32
#define M1_IN4  33
#define M1_ENB  14

// --- L298N #2 Pin Definitions (Back Motors) ---
#define M2_ENA  13
#define M2_IN1  23
#define M2_IN2  4
#define M2_IN3  18
#define M2_IN4  19
#define M2_ENB  21

// Packet format matching transmitter
typedef struct struct_message {
  char command; // 'F', 'B', 'L', 'R', 'S'
  int speed;    // 0 to 255
} struct_message;

struct_message incomingData;

void setMotorSpeed(int speedVal) {
  analogWrite(M1_ENA, speedVal);
  analogWrite(M1_ENB, speedVal);
  analogWrite(M2_ENA, speedVal);
  analogWrite(M2_ENB, speedVal);
}

void moveForward(int speedVal) {
  setMotorSpeed(speedVal);
  digitalWrite(M1_IN1, HIGH); digitalWrite(M1_IN2, LOW);
  digitalWrite(M1_IN3, HIGH); digitalWrite(M1_IN4, LOW);
  digitalWrite(M2_IN1, HIGH); digitalWrite(M2_IN2, LOW);
  digitalWrite(M2_IN3, HIGH); digitalWrite(M2_IN4, LOW);
}

void moveBackward(int speedVal) {
  setMotorSpeed(speedVal);
  digitalWrite(M1_IN1, LOW);  digitalWrite(M1_IN2, HIGH);
  digitalWrite(M1_IN3, LOW);  digitalWrite(M1_IN4, HIGH);
  digitalWrite(M2_IN1, LOW);  digitalWrite(M2_IN2, HIGH);
  digitalWrite(M2_IN3, LOW);  digitalWrite(M2_IN4, HIGH);
}

void turnLeft(int speedVal) {
  setMotorSpeed(speedVal);
  digitalWrite(M1_IN1, LOW);  digitalWrite(M1_IN2, HIGH);
  digitalWrite(M1_IN3, HIGH); digitalWrite(M1_IN4, LOW);
  digitalWrite(M2_IN1, LOW);  digitalWrite(M2_IN2, HIGH);
  digitalWrite(M2_IN3, HIGH); digitalWrite(M2_IN4, LOW);
}

void turnRight(int speedVal) {
  setMotorSpeed(speedVal);
  digitalWrite(M1_IN1, HIGH); digitalWrite(M1_IN2, LOW);
  digitalWrite(M1_IN3, LOW);  digitalWrite(M1_IN4, HIGH);
  digitalWrite(M2_IN1, HIGH); digitalWrite(M2_IN2, LOW);
  digitalWrite(M2_IN3, LOW);  digitalWrite(M2_IN4, HIGH);
}

void stopMotors() {
  setMotorSpeed(0);
  digitalWrite(M1_IN1, LOW); digitalWrite(M1_IN2, LOW);
  digitalWrite(M1_IN3, LOW); digitalWrite(M1_IN4, LOW);
  digitalWrite(M2_IN1, LOW); digitalWrite(M2_IN2, LOW);
  digitalWrite(M2_IN3, LOW); digitalWrite(M2_IN4, LOW);
}

// Universal receive callback compatible across all ESP32 core versions
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingDataBytes, int len) {
#else
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataBytes, int len) {
#endif
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  
  Serial.print("Received Command: ");
  Serial.print(incomingData.command);
  Serial.print(" | Speed: ");
  Serial.println(incomingData.speed);

  switch (incomingData.command) {
    case 'F': moveForward(incomingData.speed);  break;
    case 'B': moveBackward(incomingData.speed); break;
    case 'L': turnLeft(incomingData.speed);     break;
    case 'R': turnRight(incomingData.speed);    break;
    case 'S':
    default:  stopMotors();                     break;
  }
}

void setup() {
  // Initialize Serial Monitor first
  Serial.begin(115200);
  delay(1000); 
  Serial.println("\n--- Receiver Booting ---");

  // Motor pin configurations
  pinMode(M1_ENA, OUTPUT); pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M1_IN3, OUTPUT); pinMode(M1_IN4, OUTPUT); pinMode(M1_ENB, OUTPUT);

  pinMode(M2_ENA, OUTPUT); pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT);
  pinMode(M2_IN3, OUTPUT); pinMode(M2_IN4, OUTPUT); pinMode(M2_ENB, OUTPUT);

  stopMotors();
  Serial.println("Motor pins initialized.");

  // Wi-Fi Station Mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW!");
    return;
  }
  Serial.println("ESP-NOW Initialized Successfully.");

  // Register callback
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Ready to receive data from transmitter.");
}

void loop() {
  // Processing happens via callback
}
