#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

#define MPU9250_ADDR 0x68

// Receiver MAC Address(40:91:51:1A:37:08)
//change the MAC address with your reciver esp's mac address
uint8_t receiverAddress[] = {0x40, 0x91, 0x51, 0x1A, 0x37, 0x08};

typedef struct struct_message {
  char command; // 'F', 'B', 'L', 'R', 'S'
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void writeByte(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);

  // Initialize I2C bus (SDA: GPIO21, SCL: GPIO22)
  Wire.begin(21, 22);

  // Wake up MPU9250
  writeByte(0x6B, 0x00);
  delay(100);

  // Set Accelerometer range to +/- 2g
  writeByte(0x1C, 0x00);

  // Station mode for ESP-NOW
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    return;
  }

  // Register receiver peer
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Transmitter Ready! Tilting the sensor will display values below:");
}

void loop() {
  // Read accelerometer data (registers 0x3B to 0x40)
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU9250_ADDR, 6, true);

  if (Wire.available() >= 6) {
    int16_t rawX = (Wire.read() << 8) | Wire.read();
    int16_t rawY = (Wire.read() << 8) | Wire.read();
    int16_t rawZ = (Wire.read() << 8) | Wire.read();

    float ax = (float)rawX / 16384.0;
    float ay = (float)rawY / 16384.0;

    // Gesture control mapping
    if (ax > 0.40) {
      myData.command = 'F'; // Tilt Forward
    } else if (ax < -0.40) {
      myData.command = 'B'; // Tilt Backward
    } else if (ay > 0.40) {
      myData.command = 'R'; // Tilt Right
    } else if (ay < -0.40) {
      myData.command = 'L'; // Tilt Left
    } else {
      myData.command = 'S'; // Flat / Stop
    }

    Serial.print("X: ");
    Serial.print(ax, 2);
    Serial.print(" | Y: ");
    Serial.print(ay, 2);
    Serial.print(" -> Cmd: ");
    Serial.print(myData.command);
  } else {
    Serial.print("Warning: No I2C response from MPU9250");
  }

  // Send packet and check delivery result directly
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println(" | Delivery: Sent");
  } else {
    Serial.println(" | Delivery: Error");
  }

  delay(100);
}
