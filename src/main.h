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

/* -------------------------------------------------------------------------- */
/*                                   Macros                                   */
/* -------------------------------------------------------------------------- */
#define LED 2
#define LED_RGB_R 32
#define LED_RGB_G 33
#define LED_RGB_B 25
#define LED_CCT_C 18
#define LED_CCT_W 19

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
WiFiClient espClient;
PubSubClient mqttClient(espClient);

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
void MQTT_reconnect();

void IoT_device_init();
int randomInt();
bool randomBool();

#endif