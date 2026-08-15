Smart Water Tank Monitor

An IoT-based smart water tank monitoring and automatic pump control system using ESP32.

## 📌 Project Overview

This project monitors the water level of a tank using an ultrasonic sensor and automatically controls a water pump using a relay. The system displays the water level on a 16x2 I2C LCD and sends monitoring information to the Blynk IoT platform.

The project also includes a pH sensor for water quality monitoring. The pH monitoring functionality can be enabled in the code and displayed through Blynk.

## 🚀 Features

- 💧 Real-time water level monitoring
 Ultrasonic water level sensor
 Automatic water pump control
 Blynk IoT mobile monitoring
 16x2 I2C LCD display
 Buzzer alert when the tank is full
 LED pump status indicator
 pH sensor for water quality monitoring
 Water level percentage calculation

## 🛠️ Hardware Components

- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- 16x2 I2C LCD
- Relay Module
- Water Pump
- Buzzer
- LED
- pH Sensor
- Jumper Wires
- Power Supply

## 💻 Software & Technologies

- Arduino IDE
- C/C++
- ESP32
- Blynk IoT
- I2C LCD
- Ultrasonic Sensor

## ⚙️ Water Level Logic

| Water Level | Pump |
|----------|-------------|------|
| 0% | ON |
| 25% | ON |
| 50% | ON |
| 75% | OFF/Waiting |
| 100% | OFF |

The system automatically turns the pump on when the water level becomes low and turns it off when the tank reaches the full level.

## 📱 Blynk

The system uses Blynk IoT to monitor:

- Water level percentage
- Pump status
- pH value

### Virtual Pins

- `V0` - Pump Status
- `V1` - Water Level
- `V2` - pH Value

## 🔌 Pin Configuration

| Component | ESP32 Pin |
|-----------|-----------|
| Ultrasonic TRIG | GPIO 5 |
| Ultrasonic ECHO | GPIO 18 |
| Relay | GPIO 32 |
| Buzzer | GPIO 14 |
| LED | GPIO 2 |
| pH Sensor | GPIO 34 |
| LCD SDA | GPIO 21 |
| LCD SCL | GPIO 22 |

## 📂 Project Structure

```text
Smart-Water-Tank-Monitor/
│
├── Smart-Water-Tank-Monitor.ino
└── README.md
