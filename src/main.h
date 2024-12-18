#ifndef _MAIN_H
#define _MAIN_H

/* -------------------------------------------------------------------------- */
/*                                Include Files                               */
/* -------------------------------------------------------------------------- */
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <Preferences.h>
#include <PubSubClient.h>
#include "definitions.h"
#include "iot_cli.h"
#include "IoT_device.h"
#include <WebServer_WT32_ETH01.h>
/* -------------------------------------------------------------------------- */
/*                                   Macros                                   */
/* -------------------------------------------------------------------------- */
#define DEBUG_ETHERNET_WEBSERVER_PORT Serial
#define _ETHERNET_WEBSERVER_LOGLEVEL_ 3

#define LED 2
#define LED_RGB_R 4
#define LED_RGB_G 12
#define LED_RGB_B 14
#define LED_CCT_C 15
#define LED_CCT_W 17


/* -------------------------------------------------------------------------- */
/*                                Enumerations                                */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Variables                                 */
/* -------------------------------------------------------------------------- */
unsigned long timestamp = 0;

/* ----------------------- MQTT client default values ----------------------- */
char mqtt_server[40];
char mqtt_port[6] = "";
char mqtt_user[34] = "";
char mqtt_password[34] = "";

/* -----------------------  WiFiManager MQTT Definition ---------------------- */
WiFiManager wm;
WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
WiFiManagerParameter custom_mqtt_user("mqtt_user", "MQTT Username", mqtt_user, 32);
WiFiManagerParameter custom_mqtt_password("mqtt_password", "MQTT Password", mqtt_password, 32);

/* -------------- Preference Object to store data in the memory ------------- */
Preferences preferences;

/* ------------------------------- MQTT Client ------------------------------ */
WiFiClient    espClient;
PubSubClient  wifi_mqttClient(espClient);
WiFiClient    ethClient;
PubSubClient  eth_mqttClient(ethClient);


/* ------------------------------- IoT Device ------------------------------- */
Device myIoTdevice;
auto mySwitch = std::make_shared<Switch>("mySwitch", LED);
auto mySensor = std::make_shared<Sensor>("mySensor", _HASSIO_DEVICE_CLASS_SENSOR_VOLTAGE);
auto myAction = std::make_shared<Sensor>("Action");
// Assuming the pin assignments are as follows:
 
auto myLight = std::make_shared<Light>("RGBWW_LED", LED_RGB_R, LED_RGB_G, LED_RGB_B, LED_CCT_C, LED_CCT_W);


/* -------------------------------------------------------------------------- */
/*                               Data Structures                              */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Typedefs                                  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                  Functions                                 */
/* -------------------------------------------------------------------------- */

void wm_init(bool _reset);
void saveConfigCallback();

void MQTT_init();
void MQTT_callback(char *topic, byte *message, unsigned int length);
void MQTT_reconnect(PubSubClient &_client);

void IoT_device_init();
int randomInt();
bool randomBool();


#endif