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
git clone https://github.com/MarcoGerladi/ESP32_HassIO.git
```

## Setting Up the Environment
- Open Arduino IDE or PlatformIO.
- Install the necessary libraries:
  - PubSubClient
  - WiFiManager (https://github.com/tzapu/WiFiManager)
  - Preferences
- Upload the code to your ESP32 board.

## Flahing procedure
The Code is compatibile with multiple development board, before flashing the software make sure to select the proper enviroment. 

For this reason, go to platformIO and select the correspondent development board:

![image](https://github.com/user-attachments/assets/51f8a1df-b895-4d76-b12f-69d333a1eee8)


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

The ESP32 automatically connects to the MQTT Broker. In case the connection is lost, the ESP32 will retry to connect to the broker.

If the board used has an ethernet interface, the connection automatically switches from WiFi to Ethernet and viceversa, giving priority to the ethernet connection.

The ESP32 communicates with the MQTT broker by subscribing and publishing to specific topics.

### Device Definition

The Library allows the user to define easily entities based on the official HomeAssistant documentation for MQTT Discovery (https://www.home-assistant.io/integrations/mqtt/).

First of all, define a `Device` object followed by all the `entities` that will be shared with home assistant. The `Device` object this will be responsible of the initialization and configuration of each entity.

```cpp
/* ------------------------------- IoT Device ------------------------------- */
Device myIoTdevice;
auto mySwitch = std::make_shared<Switch>("mySwitch", LED);
auto mySensor = std::make_shared<Sensor>("mySensor", _HASSIO_DEVICE_CLASS_SENSOR_VOLTAGE);
auto myAction = std::make_shared<Sensor>("Action");
// Assuming the pin assignments are as follows:
 
auto myLight = std::make_shared<Light>("RGBWW_LED", LED_RGB_R, LED_RGB_G, LED_RGB_B, LED_CCT_C, LED_CCT_W);
```

## Device Init

Home Assistant requires that all the entities send a configuration message, this is handled by the `Device` Class and for this reason the method `IoT_device_init()` in the main.cpp file have been added. In order to configure properly the device, just use the `addEntity()` method to include all the entities in the device object and then the `configure()` method to send the configuration message to the broker.

```cpp
void IoT_device_init()
{
  /* ---------------- add all entities to the iot device object --------------- */
  mySwitch->setDeviceClass(_HASSIO_DEVICE_CLASS_SWITCH_OUTLET);

  myIoTdevice.addEntity(mySwitch);
  myIoTdevice.addEntity(mySensor);
  myIoTdevice.addEntity(myAction);
  myIoTdevice.addEntity(myLight);

  /* -------------------------- Configure IoT Device -------------------------- */
  if (eth_mqttClient.connected()){
    #ifdef ETHERNET_ENABLE 
    myIoTdevice.configure(eth_mqttClient); // Update device configuration
    #endif
  }   
  else if (wifi_mqttClient.connected()){
    myIoTdevice.configure(wifi_mqttClient); // Update device configuration
  }
  else
    Serial.println("Failed to send Device Configuration via MQTT. Client disconnected.");
}
```

## Device Update

It is up to the user to decide when to update the device data, usually this should be done:
- periodically
- on connection to the broker
- when an input is received

```cpp
 if (currentMillis - sensorUpdateTimestamp >= sensorInterval)
 {
   sensorUpdateTimestamp = currentMillis;
   mySensor->setStatus((double)rand());
   deviceUpdate();
 }
```

To update the information, simply use the `update` function of the device object 

```cpp
void deviceUpdate()
{
  /* -------------------------- update device status -------------------------- */
  if (eth_mqttClient.connected())
  {
    myIoTdevice.update(eth_mqttClient);
  }
  else if (wifi_mqttClient.connected())
  {
    myIoTdevice.update(wifi_mqttClient);
  }
  else
  {
    //Serial.println("Client disconnected");
  }
}
```

### Update Entities
Most of the entities update themselves automatically, but in case such as `sensor` entities, the value shall be updated by the user.

## Home Assitant Discovery

After flashing, make sure to configure propertly the credential of the MQTT broker, then if everything was set up correctly, you can navigate to your MQTT devices list and find the just created device

![image](https://github.com/user-attachments/assets/c742efbe-7ccf-460f-861b-04eae12a9f71)

![image](https://github.com/user-attachments/assets/e63f98da-6528-4356-ac75-e2d6601a31b8)



