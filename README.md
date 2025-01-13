# Solder Station Usage Visualizer

A comprehensive IoT project designed to visualize the usage status of soldering workstations through color-coded lights and dynamic animations. This project combines hardware and software to demonstrate the power of IoT in enhancing workspace efficiency and interaction.

---

## Table of Contents
- [Introduction](#introduction)
- [Hardware Components](#hardware-components)
- [Software Details](#software-details)
- [System Setup](#system-setup)
- [Features](#features)
- [Future Enhancements](#future-enhancements)

<img src="D:\UCL_CE_Term1\CASA0014\CASA0014_Final\Pictures\picture1.png" width="500" height="200"/> 
---

## Introduction

The **Solder Station Usage Visualizer** is a simple yet effective tool to solve a common problem in shared workspaces: understanding station availability at a glance. By leveraging an ultrasonic sensor, an MKR1010 development board, and MQTT protocol, the system provides:

- **Pink Light**: Indicates the station is free.
- **Yellow Light**: Shows someone is nearby but not actively using the station.
- **Blue Light**: Signals the station is in use, with a dynamic wave animation for added interactivity.

---

## Hardware Components

### 1. Ultrasonic Sensor (HC-SR04)
- **Purpose**: Measures the distance between the user and the station.
- **Connection**:
  - **Trig Pin**: Connected to MKR1010 Pin 1.
  - **Echo Pin**: Connected to MKR1010 Pin 2.
  - **Power**: VCC to 5V, GND to GND.

### 2. MKR1010 Development Board
- **Purpose**: Processes distance data and transmits status information via MQTT.
- **Special Features**:
  - Integrated WiFi for seamless IoT connectivity.
  - Compact and versatile for prototyping.

### 3. NeoPixel LED Ring (12 Pixels)
- **Purpose**: Displays the workspace status through color-coded lights and animations.
- **Connection**:
  - **Data Pin**: Connected to MKR1010 Pin 6.
  - **Power**: VCC to 5V, GND to GND.

### 4. Supporting Components
- Breadboard and jumper wires for connections.
- USB power supply (5V) for the MKR1010.


<img src="D:\UCL_CE_Term1\CASA0014\CASA0014_Final\Pictures\picture2.png" width="500" height="200"/> 
---

## Software Details

### Programming Environment
- **Arduino IDE**: Used to write and upload the firmware to the MKR1010 board.

### Libraries Used
1. **WiFiNINA**: Handles WiFi connectivity.
2. **PubSubClient**: Enables MQTT communication.
3. **Adafruit NeoPixel**: Controls the LED animations.

### Key Functionalities
1. **Data Collection**:
   - Ultrasonic sensor measures distance and transmits readings to the MKR1010 board.
2. **Data Processing**:
   - Distance readings are mapped to workspace statuses.
   - Color and animation logic is determined based on these statuses.
3. **Data Transmission**:
   - MQTT protocol sends data to the NeoPixel LED ring for real-time visualization.

### Code Overview
The system follows a simple workflow:
- **Initialization**: Configures WiFi, MQTT, and sensors.
- **Loop**:
  1. Measures distance.
  2. Determines the appropriate status.
  3. Sends color and animation commands to the LED ring.

---

## System Setup

### Hardware Setup
1. Connect the **ultrasonic sensor** to the MKR1010 board:
   - Trig Pin to Pin 1
   - Echo Pin to Pin 2
   - VCC to 5V
   - GND to GND
2. Connect the **NeoPixel LED ring**:
   - Data Pin to Pin 6
   - VCC to 5V
   - GND to GND
3. Power the system using a USB adapter.

### Software Setup
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/solder-station-visualizer.git
   cd solder-station-visualizer
   ```
2. Open the `.ino` file in Arduino IDE.
3. Configure `arduino_secrets.h` with your WiFi and MQTT credentials:
   ```c
   #define SECRET_SSID "your_wifi_ssid"
   #define SECRET_PASS "your_wifi_password"
   #define SECRET_MQTTUSER "your_mqtt_username"
   #define SECRET_MQTTPASS "your_mqtt_password"
   ```
4. Upload the code to the MKR1010 board.

---

## Features

- **Real-Time Visualization**: Color-coded lights for instant status feedback.
- **Dynamic Animation**: Engaging wave effect when the station is in use.
- **Remote Monitoring**: Uses MQTT for efficient data transmission.
- **Scalable Design**: Easily expandable to monitor multiple stations.

---

## Future Enhancements

- **Improved Sensors**: Replace ultrasonic sensors with more accurate options like infrared or laser.
- **Data Analytics**: Upload usage data to the cloud for detailed reporting.
- **Enhanced Security**: Implement encrypted MQTT communication to prevent unauthorized access.
- **Mobile Integration**: Develop a companion app for remote monitoring and control.

---

