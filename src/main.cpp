#include "main.h"

// Create the list of commands
tinyCLI::Command commands[] = {
    {CLI_HELP, "Shows this help message", nullptr}, // Placeholder for help function
    {CLI_RESET, "Reset the device", cli_reset},
    {CLI_MQTT_INFO, "Prints stored mqtt settings", cli_mqtt_info},
    {CLI_MQTT_SERVER, "Set MQTT broker IP address", cli_mqtt_server},
    {CLI_MQTT_PORT, "Set MQTT broker port", cli_mqtt_port},
    {CLI_MQTT_USER, "Set MQTT username", cli_mqtt_user},
    {CLI_MQTT_PASSWORD, "Set MQTT password", cli_mqtt_password},
    {CLI_WIFI_CONFIG_PORTAL, "Enable Config Portal", cli_config_portal},
    {CLI_WIFI_SSID, "Set WiFi SSID", nullptr},
    {CLI_WIFI_PSWD, "Set WiFi Password", nullptr},

};

// Initialize the command line interface
tinyCLI commandLine(Serial, commands, sizeof(commands) / sizeof(commands[0]));

void setup()
{

  /* --------------------- Initialize Serial Communication -------------------- */
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT);

  /* -------------------------- Initialize Parameters ------------------------- */
  preferences.begin("my-app", false);

  /* ------------------------ Initiliaze WiFi Settings ------------------------ */
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  /* --------------------- Initialize WiFi Manager Object --------------------- */
  wm_init(false);

  /* ----------------------- Initialize MQTT Parameters ----------------------- */
  MQTT_init();

  /* -------------------------- Initialize IoT Device ------------------------- */
  IoT_device_init();
}

void loop()
{

  /* ----------------------- Process Command line inputs ---------------------- */
  commandLine.processInput();

  /* -------------------------- WiFi manager Process -------------------------- */
  wm.process();

  /* ------------------------- Connect to MQTT Broker ------------------------- */
  if (WL_CONNECTED == WiFi.status())
  {
    /* --------------------- Try to reconnect to MQTT Broker -------------------- */
    if (!mqttClient.connected())
    {
      if (0 == (millis() % 5000) && timestamp != millis())
      {
        MQTT_reconnect();
      }
    }
    else
    {
      /* ---------------------------- Publish MQTT Data --------------------------- */
      mqttClient.loop();
    }
  }

  if (0 == (millis() % 1000) && timestamp != millis())
  {

    mySensor->setStatus((double)rand());

    myIoTdevice.update(mqttClient);
  }

  /* ------------------------ update timestamp each ms ------------------------ */
  if (timestamp != millis())
    timestamp = millis();

  /* -------------------------- Update MQTT Entities -------------------------- */
}

void IoT_device_init()
{

  /* ---------------- add all entities to the iot device object --------------- */
  mySwitch->setDeviceClass(_HASSIO_DEVICE_CLASS_SWITCH_OUTLET);

  myIoTdevice.addEntity(mySwitch);
  myIoTdevice.addEntity(mySensor);
  myIoTdevice.addEntity(myAction);

  myIoTdevice.addEntity(myLight);

  /* -------------------------- Configure IoT Device -------------------------- */
  myIoTdevice.configure(mqttClient); // Update device configuration
}


void MQTT_callback(char *topic, byte *message, unsigned int length)
{

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  Serial.println();

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == mySwitch->getCommandTopic())
  {

    if (messageTemp == mySwitch->getPayloadOn())
    {
      digitalWrite(LED, HIGH);
      myAction->setStatus("led_on");
    }
    else if (mySwitch->getPayloadOff())
    {
      digitalWrite(LED, LOW);
      myAction->setStatus("led_off");
    }
  }

  if (String(topic) == myLight -> getCommandTopic()){
    myLight -> setStatus(messageTemp);
  }

  /* -------------------------- update device status -------------------------- */
  myIoTdevice.update(mqttClient);
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void saveConfigCallback()
{
  /* --------------------------- Store MQTT settings -------------------------- */
  preferences.putString(PREF_MQTT_SERVER, custom_mqtt_server.getValue());
  preferences.putString(PREF_MQTT_PORT, custom_mqtt_port.getValue());
  preferences.putString(PREF_MQTT_USER, custom_mqtt_user.getValue());
  preferences.putString(PREF_MQTT_PASSWORD, custom_mqtt_password.getValue());
}

void MQTT_init()
{

  /* ----------------------- Initialize MQTT Parameters ----------------------- */
  String _mqtt_port = preferences.getString(PREF_MQTT_PORT, mqtt_port);
  String _mqtt_server = preferences.getString(PREF_MQTT_SERVER, mqtt_server);
  String _mqtt_user = preferences.getString(PREF_MQTT_USER, mqtt_user);
  String _mqtt_password = preferences.getString(PREF_MQTT_PASSWORD, mqtt_password);

  /* ---------------------- Remove whitespaces at the end --------------------- */
  _mqtt_user.trim();
  _mqtt_password.trim();
  _mqtt_port.trim();
  _mqtt_server.trim();

  custom_mqtt_server.setValue(_mqtt_server.c_str(), 128);
  custom_mqtt_port.setValue(_mqtt_port.c_str(), 128);
  custom_mqtt_user.setValue(_mqtt_user.c_str(), 128);
  custom_mqtt_password.setValue(_mqtt_password.c_str(), 128);

  mqttClient.setServer(custom_mqtt_server.getValue(), 1883);
  mqttClient.setCallback(MQTT_callback);

  /* ------------------------- Connect to MQTT Broker ------------------------- */
  mqttClient.connect("ESP8266Client99", _mqtt_user.c_str(), _mqtt_password.c_str());
}

void wm_init(bool _reset)
{

  if (_reset)
    wm.resetSettings(); // reset settings - wipe credentials for testing

  wm.addParameter(&custom_mqtt_server);
  wm.addParameter(&custom_mqtt_port);
  wm.addParameter(&custom_mqtt_user);
  wm.addParameter(&custom_mqtt_password);

  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(60);

  // automatically connect using saved credentials if they exist
  // If connection fails it starts an access point with the specified name
  if (wm.autoConnect("AutoConnectAP"))
  {
    Serial.println("connected...yeey :)");
  }
  else
  {
    Serial.println("Failed to connect");
  }

  // set config save notify callback
  wm.setSaveParamsCallback(saveConfigCallback);
}

void MQTT_reconnect()
{

  /* ------------------------ Retrieve data from memory ----------------------- */
  String _mqtt_user = preferences.getString(PREF_MQTT_USER, mqtt_user);
  String _mqtt_password = preferences.getString(PREF_MQTT_PASSWORD, mqtt_password);
  String _mqtt_port = preferences.getString(PREF_MQTT_PORT, mqtt_port);
  String _mqtt_server = preferences.getString(PREF_MQTT_SERVER, mqtt_server);

  /* ---------------------- Remove whitespaces at the end --------------------- */
  _mqtt_user.trim();
  _mqtt_password.trim();
  _mqtt_port.trim();
  _mqtt_server.trim();

  if (!mqttClient.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    mqttClient.setServer(_mqtt_server.c_str(), _mqtt_port.toInt());

    if (mqttClient.connect("ESP8266Client99", _mqtt_user.c_str(), _mqtt_password.c_str()))
    {
      Serial.println("connected");

      /* --------------------- Update IoT Device configuration -------------------- */
      myIoTdevice.configure(mqttClient); // Update device configuration
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
    }
  }
}


int randomInt()
{
  return rand();
}

bool randomBool()
{
  return rand() % 2;
}
