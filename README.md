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
1. Flash `receiver/receiver.ino` to the car's ESP32.
2. Open the Serial Monitor to read the Receiver MAC address.
3. Update the MAC address array in `transmitter/transmitter.ino`.
4. Flash `transmitter/transmitter.ino` to the remote ESP32.
