# ESP32 Dual L298N RC Car (ESP-NOW)

A 4-wheel drive RC vehicle powered by two ESP32 DevKit boards communicating over ESP-NOW and driving two L298N motor drivers.

## Features
- Peer-to-peer low-latency control using ESP-NOW
- Dual L298N H-bridges (Front & Rear control)
- PWM speed control via ENA/ENB

## Pinout Configuration
### L298N #1 (Front)
- ENA: GPIO 25
- IN1: GPIO 26
- IN2: GPIO 27
- IN3: GPIO 32
- IN4: GPIO 33
- ENB: GPIO 14

### L298N #2 (Rear)
- ENA: GPIO 13
- IN1: GPIO 23
- IN2: GPIO 4
- IN3: GPIO 18
- IN4: GPIO 19
- ENB: GPIO 21

## Setup 
### Step 1: Retrieve the Receiver MAC Address

ESP-NOW requires the transmitter to know the exact hardware MAC address of the receiver.

1. Connect the **Receiver ESP32** to your computer via USB.
2. Open `tools/get_mac_address/get_mac_address.ino` in Arduino IDE.
3. Select your board under **Tools > Board > DOIT ESP32 DEVKIT V1** and select the proper COM port.
4. Upload the sketch.
5. Open the **Serial Monitor** (**Ctrl + Shift + M** / **Cmd + Shift + M**) and set the baud rate to **115200**.
6. Press the **EN / RST** button on the ESP32.
7. Copy the printed MAC address (e.g., `40:91:51:1A:37:08`).

---

### Step 2: Configure and Flash the Transmitter

1. Open `transmitter/transmitter.ino`.
2. Update the MAC address array in `transmitter/transmitter.ino`.
3. Flash `transmitter/transmitter.ino` to
