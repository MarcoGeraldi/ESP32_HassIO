# ESP32 MQTT Client with Configuration Portal and CLI

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Getting Started](#getting-started)
   - [Hardware Requirements](#hardware-requirements)
   - [Software Requirements](#software-requirements)
4. [Installation](#installation)
   - [Cloning the Repository](#cloning-the-repository)
   - [Setting Up the Environment](#setting-up-the-environment)
5. [Usage](#usage)
   - [Configuration Portal](#configuration-portal)
   - [CLI Commands](#cli-commands)
6. [MQTT Communication](#mqtt-communication)

## Introduction
This project demonstrates how to use an ESP32 board as an MQTT client, allowing easy configuration via a web portal and providing access to key features through a serial CLI. This simplifies both wireless communication and device management.

## Features
- MQTT client functionality with ESP32
- Web-based configuration portal for network and MQTT settings
- Basic CLI over serial for easy command access
- Lightweight and customizable

## Getting Started

### Hardware Requirements
- ESP32 development board
- USB cable

![image](https://github.com/user-attachments/assets/94458f26-6eeb-4d60-a9dd-23cb3c0074d0)

### Software Requirements
- Arduino IDE or PlatformIO
- ESP32 core installed in the Arduino IDE
- MQTT broker (e.g., Mosquitto)
- Libraries:
  - PubSubClient (for MQTT)
  - WiFiManager (https://github.com/tzapu/WiFiManager)
  - Preferences (for saving settings)

## Installation

### Cloning the Repository
To clone the repository, use the following command:

```bash
git clone https://github.com/MarcoGerladi/ESP32MqttExample.git
```

## Setting Up the Environment
- Open Arduino IDE or PlatformIO.
- Install the necessary libraries:
  - PubSubClient
  - WiFiManager (https://github.com/tzapu/WiFiManager)
  - Preferences
- Upload the code to your ESP32 board.


## Usage

### Configuration Portal

- Once the ESP32 is powered on, it will create a WiFi access point (AP).
- Connect to the AP using your phone or computer.
- Open a browser and navigate to the ESP32’s IP address (usually 192.168.4.1).
- Enter your WiFi network details and MQTT broker information.
- At this point the ESP32 will stop the configuration portal. in order to restart it, execute the `config_portal` command via the `CLI`

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/b8a243b5-eee7-4aec-870f-244f5c6eea66" ></td>
    <td><img src="https://github.com/user-attachments/assets/5e01c806-211c-4fd7-b2d4-5ed0606e740b" ></td>
  </tr>
</table>

### CLI Commands
 
Use the serial monitor to access the CLI. Here are the available commands:
- `help`: Shows the list of the available commands
- `cli_reset`: Reset the device
- `cli_mqtt_info`: Prints stored mqtt settings
- `cli_mqtt_server`: Set MQTT broker IP address
- `cli_mqtt_port`: Set MQTT broker port
- `cli_mqtt_user`: Set MQTT username
- `cli_mqtt_password`: Set MQTT password
- `cli_config_portal`: Enable Config Portal

### MQTT Communication

The ESP32 automatically connects to the MQTT Broker. In case the connection is lost, the ESP32 will retry to connect to the broker each 5s.

``` cpp
      /* --------------------- Try to reconnect to MQTT Broker -------------------- */
      if ( millis() - timestamp  > 5000) {
          MQTT_reconnect();   
          timestamp = millis();
        }
```

The ESP32 communicates with the MQTT broker by subscribing and publishing to specific topics.

- Publish exmaple:

Publish Topic: esp32/temperature

``` cpp   
  /* ---------------------------- Publish MQTT Data --------------------------- */
        mqttClient.loop();
        mqttClient.publish("esp32/temperature", "19°C");
```

![image](https://github.com/user-attachments/assets/1fdd8d23-f873-4f75-9cae-6246ab84fbb5)

- Subscribe Example:

  Subscribed Topic: esp32/output

``` cpp   
  mqttClient.subscribe("esp32/output");
```

![image](https://github.com/user-attachments/assets/36f3a5d6-16bf-4a17-a3b9-7b9900ce9dac)

![image](https://github.com/user-attachments/assets/3682e1e6-b96f-4894-93e9-7268c1fac371)

