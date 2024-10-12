#include "iot_cli.h"

void controlLED(String params) {
  params.trim();
  if (params == "-on") {
    Serial.println("LED is now ON!");
    // Add code to turn on the LED here
  } else if (params == "-off") {
    Serial.println("LED is now OFF!");
    // Add code to turn off the LED here
  } else {
    Serial.println("Unknown parameter. Use '-on' or '-off'.");
  }
}

void sayHello(String params) {
  Serial.println("Hello, World!");
}

void cli_reset(String params){
    Serial.println("resetting..");
    ESP.restart();
}

void cli_mqtt_info(String params){
    Serial.println("MQTT Broker IP Address: "   + preferences.getString(PREF_MQTT_SERVER,   ""));
    Serial.println("MQTT Port: "        + preferences.getString(PREF_MQTT_PORT,     ""));
    Serial.println("MQTT Username: "    + preferences.getString(PREF_MQTT_USER,     ""));
    Serial.println("MQTT Password: "    + preferences.getString(PREF_MQTT_PASSWORD, ""));
}

void cli_mqtt_server(String params){
    
    params.trim();    

    if (params == CLI_MQTT_SERVER){
        Serial.println("MQTT Broker IP Address: "   + preferences.getString(PREF_MQTT_SERVER,   ""));
        Serial.println("To set a new IP address, write " CLI_MQTT_SERVER " followed by the new IP Address");
    } else {
        preferences.putString(PREF_MQTT_SERVER,    params.c_str());
        Serial.println("New MQTT Broker IP Address: "   + preferences.getString(PREF_MQTT_SERVER,   ""));
    }
}
 
void cli_mqtt_port(String params){
    
    params.trim();
    
    if (params == CLI_MQTT_PORT){
        Serial.println("MQTT Broker Port: "   + preferences.getString(PREF_MQTT_PORT,   ""));
        Serial.println("To set a new port, write " CLI_MQTT_PORT " followed by the new port number");
    } else {
        preferences.putString(PREF_MQTT_PORT,    params.c_str());
        Serial.println("New MQTT Port: "   + preferences.getString(PREF_MQTT_PORT,   ""));
    }
}

void cli_mqtt_user(String params){
    params.trim();
    
    if (params == CLI_MQTT_USER){
        Serial.println("MQTT Username: "   + preferences.getString(PREF_MQTT_USER,   ""));
        Serial.println("To set a new username, write " CLI_MQTT_USER " followed by the new username");
    } else {
        preferences.putString(PREF_MQTT_USER,    params.c_str());
        Serial.println("New MQTT Username: "   + preferences.getString(PREF_MQTT_USER,   ""));
    }
}

void cli_mqtt_password(String params){
    
    params.trim();
    
    if (params == CLI_MQTT_PASSWORD){
        Serial.println("MQTT Password: "   + preferences.getString(PREF_MQTT_PASSWORD,   ""));
        Serial.println("To set a new password, write " CLI_MQTT_PASSWORD " followed by the new password");
    } else {
        preferences.putString(PREF_MQTT_PASSWORD,    params.c_str());
        Serial.println("New MQTT Password: "   + preferences.getString(PREF_MQTT_PASSWORD,   ""));
    }
}

void cli_config_portal(String params){
    params.trim();
    
    /* --------------------------- Start Config Portal -------------------------- */
    wm.startConfigPortal("OnDemandAP","12345678");
}