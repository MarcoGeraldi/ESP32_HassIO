#ifndef _MAIN_H
#define _MAIN_H


/* -------------------------------------------------------------------------- */
/*                                Include Files                               */
/* -------------------------------------------------------------------------- */
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <Preferences.h>
#include <PubSubClient.h>
#include "definitions.h"
#include "iot_cli.h"

/* -------------------------------------------------------------------------- */
/*                                   Macros                                   */
/* -------------------------------------------------------------------------- */
#define LED 2

/* -------------------------------------------------------------------------- */
/*                                Enumerations                                */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/*                                  Variables                                 */
/* -------------------------------------------------------------------------- */
unsigned long timestamp = 0;

/* ----------------------- MQTT client default values ----------------------- */
char mqtt_server[40];
char mqtt_port[6]      = "";
char mqtt_user[34]     = "";
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
void saveConfigCallback ();

void MQTT_paramInit();
void MQTT_callback(char* topic, byte* message, unsigned int length) ;
void MQTT_reconnect();

#endif