#include "IoT_device.h"

#include <string>
#include <cstdio>  // Per snprintf
#include <cstdlib> // Per malloc

#ifndef ESP8266
#include "esp_mac.h"
#else 
#include <ESP8266WiFi.h>
#endif
/* -------------------------------------------------------------------------- */
/*                              HELPER FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
String stringSnakeCase(const String &entityName)
{
    String output = "";

    for (char currentChar : entityName)
    {
        if (currentChar == ' ')
        {
            output += "_"; // Replace space with underscore
        }
        else
        {
            if (isUpperCase(currentChar))
            {
                if (output.length() > 0 && (isLowerCase(output.charAt(output.length() - 1)) || isDigit(output.charAt(output.length() - 1))))
                {
                    output += "_"; // Add underscore before uppercase letter
                }
                currentChar = toLowerCase(currentChar); // Convert to lowercase
            }
            output += currentChar; // Append the character
        }
    }

    return output;
}

static String stringValueTemplate(const String &entityName)
{
    String prefix = "{{value_json.";
    String suffix = "}}";

    String output = prefix + stringSnakeCase(entityName) + suffix;

    return output;
}

/* -------------------------------------------------------------------------- */
/*                                DEVICE CLASS                                */
/* -------------------------------------------------------------------------- */
Device::Device()
{
    identifiers = DEVICE_IDENTIFIER;
    manufacturer = DEVICE_MANUFACTURER;
    model = DEVICE_MODEL;
    name = DEVICE_NAME;
    sw_version = DEVICE_SW_VERS;
    
    #ifndef ESP8266

    /* -------------------------- Retrieve MAC Address -------------------------- */
    uint8_t baseMac[6];
    esp_err_t ret = esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
 
        // Convert the MAC address to a String
    String macAddress = "0x";
    for (int i = 0; i < 6; i++) {
        macAddress += String(baseMac[i], 16); // Convert each byte to hexadecimal
    }
    macAddress.toLowerCase(); // Optional: Convert to uppercase for readability
    
    serial_number = macAddress;
    
    #else
    // Retrieve the MAC address
    uint8_t mac[6];
    WiFi.macAddress(mac);

    // Convert to the desired format
    char macFormatted[14]; // Space for "0x" + 12 characters + null terminator
    snprintf(macFormatted, sizeof(macFormatted), "0x%02X%02X%02X%02X%02X%02X", 
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    serial_number = macFormatted;
    
    #endif
    
    setStateTopic();
}

void Device::setStateTopic()
{
    state_topic = manufacturer +
                  "/" + model +
                  "/" + serial_number;
}

/* ----------------------- Add entities to the device ----------------------- */
void Device::addEntity(const std::shared_ptr<Entity> &entity)
{

    // Correctly calling getSerialNumber() on the current Device instance
    entity->generateUniqueID(entity->getName(), this->getSerialNumber());

    // Update the State Topic
    entity->setStateTopic(this->getDeviceStateTopic());

    // Update the Command topic
    entity->setCommandTopic();

    String configTopic = "homeassistant/" + entity->getType() + "/" + serial_number + "/" + entity->getName() + "/config";
    entity->setConfigTopic(configTopic);

    // Configure hardware
    entity->hw_init();

    // Add the entity to the list
    entities.push_back(entity); // Storing the shared pointer
}

JsonDocument Device::getEntityStatus()
{
    JsonDocument EntitiesStatus;

    for (size_t i = 0; i < entities.size(); ++i)
    {

        const std::shared_ptr<Entity> &entityPtr = entities[i];

        // Loop through all entities and add to the JSON object
        for (const auto &entityPtr : entities)
        {
            EntitiesStatus[stringSnakeCase(entityPtr->getName())] = entityPtr->getStatus();
        }
    }

    return EntitiesStatus;
}

JsonDocument Device::getEntityConfigJSON(const std::shared_ptr<Entity> &entity)
{

    JsonDocument _entityConfig;
    JsonObject _device = _entityConfig.createNestedObject("device");

    /* ----------------------- define device configuration ---------------------- */
    _device[_HASSIO_DEVICE_IDENTIFIERS] = identifiers;
    _device[_HASSIO_DEVICE_MANUFACTURER] = manufacturer;
    _device[_HASSIO_DEVICE_MODEL] = model;
    _device[_HASSIO_DEVICE_NAME] = name;
    _device[_HASSIO_DEVICE_SW_VERSION] = sw_version;

    /* ------------------------ add entity configuration ------------------------ */
    entity->getConfigJson(_entityConfig);

    return _entityConfig;
}

String Device::getEntityConfigString(const std::shared_ptr<Entity> &entity)
{

    String entityConfig;

    serializeJsonPretty(getEntityConfigJSON(entity), entityConfig);

    return entityConfig;
}

void Device::configure(PubSubClient &_mqttClient)
{
    _mqttClient.setBufferSize(MQTT_MAX_PACKET_SIZE);

    if (_mqttClient.connected())
    {
        for (size_t i = 0; i < entities.size(); ++i)
        {

            const std::shared_ptr<Entity> &entityPtr = entities[i];

            /* ------- Loop trhough all the entities and publish the configuration ------ */
            for (const auto &entityPtr : entities)
            {
                _mqttClient.publish(entityPtr->getConfigTopic().c_str(), Device::getEntityConfigString(entityPtr).c_str());
                _mqttClient.subscribe(entityPtr->getCommandTopic().c_str());
            }
        }
    }
}

void Device::update(PubSubClient &_mqttClient)
{
    String entitiesState;

    if (_mqttClient.connected())
    {
        serializeJsonPretty(getEntityStatus(), entitiesState);
        _mqttClient.publish(state_topic.c_str(), entitiesState.c_str());

        // Publish entities with JSON status (Lights)
        for (size_t i = 0; i < entities.size(); ++i)
        {
            const std::shared_ptr<Entity> &entityPtr = entities[i];

            /* ------- Loop trhough all the entities and publish the Status ------ */
            for (const auto &entityPtr : entities)
            {
                if (entityPtr->getType() == _HASSIO_ENTITY_TYPE_LIGHT)
                {
                    _mqttClient.publish(entityPtr->getStateTopic().c_str(), entityPtr->getStatus().c_str());
                }
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                                ENTITY CLASS                                */
/*--------------------------------------------------------------------------- */

Entity::Entity(String _name, String _type)
    : name(_name), type(_type)
{
}

JsonDocument Entity::getConfigJson(JsonDocument &_entityConfig)
{
    return _entityConfig;
}

String Entity::getName()
{
    return name;
}

String Entity::generateUniqueID(const String &entityName, const String &deviceId)
{
    // Converti il nome dell'entità in snake case (funzione stringSnakeCase)
    String entityNameSnakeCase = stringSnakeCase(entityName);

    // Combina deviceId e entityNameSnakeCase
    unique_id = deviceId + "_" + entityNameSnakeCase;

    return unique_id;
}

void Entity::setStateTopic(const String &_stateTopic)
{
    state_topic = _stateTopic;
}

void Entity::setCommandTopic()
{
    command_topic = state_topic +
                    "/" + stringSnakeCase(name) +
                    "/SET";
}

void Entity::setConfigTopic(const String &_configTopic)
{
    // config_topic = "homeassistant/"+ type +"/" + unique_id +"/" + name +"/config";
    config_topic = _configTopic;
}

String Entity::getConfigTopic()
{
    return config_topic;
}

String Entity::getCommandTopic()
{
    return command_topic;
}

void Entity::setDeviceClass(const char *_deviceClass)
{
    device_class = _deviceClass;
}

/* -------------------------------------------------------------------------- */
/*                                 LIGHT CLASS                                */
/* -------------------------------------------------------------------------- */

// Base constructor
Light::Light(String _name)
    : Entity(_name, _HASSIO_ENTITY_TYPE_LIGHT)
{
    /* ---------------------- set schema to json by default --------------------- */
    schema = _HASSIO_ENTITY_LIGHT_SCHEMA_JSON;

    /* --------------------- Set brightness true by default --------------------- */
    brightness = true;

    brightness_value = 255;

    color_temp = 0;

    /* ---------------------------- Set inital state ---------------------------- */
    state = "OFF";
}

// Single color light constructor (delegates to base constructor)
Light::Light(String _name, uint8_t single_pin)
    : Light(_name)
{
    light_pin = single_pin;

    supported_color_modes.clear();
    supported_color_modes.push_back(_HASSIO_ENTITY_LIGHT_COLOR_MODE_BRIGHTNESS);

    color_mode = _HASSIO_ENTITY_LIGHT_COLOR_MODE_BRIGHTNESS;

    color_temp = 0;
}

// CCT light constructor (delegates to base constructor)
Light::Light(String _name, uint8_t _CCTCoolPin, uint8_t _CCTWarmPin)
    : Light(_name)
{

    CCT_pins[0] = _CCTCoolPin;
    CCT_pins[1] = _CCTWarmPin;

    supported_color_modes.clear();
    supported_color_modes.push_back(_HASSIO_ENTITY_LIGHT_COLOR_MODE_COLORTEMP);

    color_mode = _HASSIO_ENTITY_LIGHT_COLOR_MODE_COLORTEMP;

    color_temp = (max_mireds + min_mireds) / 2;

    cct_cool_value = 255;
    cct_warm_value = 255;
}

// RGB light constructor (delegates to base constructor)
Light::Light(String _name, uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin)
    : Light(_name)
{

    rgb_pins[0] = red_pin;
    rgb_pins[1] = green_pin;
    rgb_pins[2] = blue_pin;

    supported_color_modes.clear();
    supported_color_modes.push_back(_HASSIO_ENTITY_LIGHT_COLOR_MODE_RGB);
    color_mode = _HASSIO_ENTITY_LIGHT_COLOR_MODE_RGB;
    color_temp = 0;

    r_value = 255;
    g_value = 255;
    b_value = 255;
}

// CCT + RGB light constructor (delegates to base constructor)
Light::Light(String _name, uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, uint8_t _CCTCoolPin, uint8_t _CCTWarmPin)
    : Light(_name)
{
    rgb_pins[0] = red_pin;
    rgb_pins[1] = green_pin;
    rgb_pins[2] = blue_pin;
    CCT_pins[0] = _CCTCoolPin;
    CCT_pins[1] = _CCTWarmPin;

    supported_color_modes.clear();
    supported_color_modes.push_back(_HASSIO_ENTITY_LIGHT_COLOR_MODE_COLORTEMP);
    supported_color_modes.push_back(_HASSIO_ENTITY_LIGHT_COLOR_MODE_RGB);

    color_mode = _HASSIO_ENTITY_LIGHT_COLOR_MODE_COLORTEMP;

    color_temp = (max_mireds + min_mireds) / 2;
    cct_cool_value = 255;
    cct_warm_value = 255;
    r_value = 255;
    g_value = 255;
    b_value = 255;
}

void Light::hw_init()
{
    if (rgb_pins[0] != 0)
    {
        pinMode(rgb_pins[0], OUTPUT);
        digitalWrite(rgb_pins[0], LOW);
    }
    if (rgb_pins[1] != 0)
    {
        pinMode(rgb_pins[1], OUTPUT);
        digitalWrite(rgb_pins[1], LOW);
    }
    if (rgb_pins[2] != 0)
    {
        pinMode(rgb_pins[2], OUTPUT);
        digitalWrite(rgb_pins[2], LOW);
    }
    if (CCT_pins[0] != 0)
    {
        pinMode(CCT_pins[0], OUTPUT);
        digitalWrite(CCT_pins[0], LOW);
    }
    if (CCT_pins[1] != 0)
    {
        pinMode(CCT_pins[1], OUTPUT);
        digitalWrite(CCT_pins[1], LOW);
    }
    if (light_pin != 0)
    {
        pinMode(light_pin, OUTPUT);
        digitalWrite(light_pin, LOW);
    }
}

JsonDocument Light::getConfigJson(JsonDocument &_entityConfig)
{
    _entityConfig["brightness"] = brightness;
    _entityConfig[_HASSIO_ENTITY_COMMAND_TOPIC] = command_topic;
    _entityConfig["max_mireds"] = max_mireds;
    _entityConfig["min_mireds"] = min_mireds;
    _entityConfig[_HASSIO_ENTITY_NAME] = name;
    _entityConfig["schema"] = schema;
    _entityConfig[_HASSIO_ENTITY_STATE_TOPIC] = state_topic;
    _entityConfig[_HASSIO_ENTITY_UNIQUE_ID] = unique_id;
    _entityConfig[_HASSIO_ENTITY_VALUE_TEMPLATE] = stringValueTemplate(name);

    JsonArray modes = _entityConfig.createNestedArray("supported_color_modes");

    // Populate JSON with supported color modes
    for (const char *mode : supported_color_modes)
    {
        modes.add(mode);
    }

    return _entityConfig;
}

void Light::setStateTopic(const String &_stateTopic)
{
    state_topic = _stateTopic + "/" + unique_id;
}

String Light::getStatus()
{
    String lightState;

    JsonDocument lightStateJSON;

    // Add fields to the document
    lightStateJSON["brightness"] = brightness_value;

    // Add nested color object and its properties
    JsonObject color = lightStateJSON.createNestedObject("color");

    color["r"] = r_value;
    color["g"] = g_value;
    color["b"] = b_value;

    // Add other fields
    lightStateJSON["color_mode"] = color_mode;
    lightStateJSON["color_power_on_behavior"] = nullptr; // null value
    lightStateJSON["color_temp"] = color_temp;
    lightStateJSON["state"] = state;

    // Serialize JSON to a string and print it
    serializeJson(lightStateJSON, lightState);

    return lightState;
}

void Light::setStatus(const String &valueToSet)
{
    DynamicJsonDocument lightCommand(1024); // Adjust size as needed

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(lightCommand, valueToSet);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return; // Exit if deserialization fails
    }

    // Read State command
    if (lightCommand.containsKey("state"))
    {
        String state_cmd = lightCommand["state"];   // Read state from JSON
        state = (state_cmd == "ON") ? "ON" : "OFF"; // Update status based on state
    }

    if (state != "ON")
    {
        /* ----------------------------- Turn off Light ----------------------------- */
        if (rgb_pins[0] != 0)
            analogWrite(rgb_pins[0], 0);

        if (rgb_pins[1] != 0)
            analogWrite(rgb_pins[1], 0);

        if (rgb_pins[2] != 0)
            analogWrite(rgb_pins[2], 0);

        if (CCT_pins[0] != 0)
            analogWrite(CCT_pins[0], 0);

        if (CCT_pins[1] != 0)
            analogWrite(CCT_pins[1], 0);

        if (light_pin != 0)
            analogWrite(light_pin, 0);
    }
    else
    {
        /* -------------------------------- Set Light ------------------------------- */
        if (lightCommand.containsKey("brightness"))
        {
            String _brightness = lightCommand["brightness"];
            setBrightness(_brightness.toInt());
        }

        if (lightCommand.containsKey("color_temp"))
        {
            String _color_temp = lightCommand["color_temp"];
            setColorTemperature(_color_temp.toInt());
        }

        if (lightCommand.containsKey("color")){
            // Extract "x" and "y" values within the "color" object
            int _rgb_r = lightCommand["color"]["r"];  
            int _rgb_g = lightCommand["color"]["g"];  
            int _rgb_b = lightCommand["color"]["b"];  
            setRGB(_rgb_r, _rgb_g, _rgb_b);
        }

        /* --------------------- In case only the state is sent -------------------- */
        if (!lightCommand.containsKey("brightness") && !lightCommand.containsKey("color_temp") && !lightCommand.containsKey("color")){
            setBrightness(brightness_value);
        }
    }
}

// Set Brightness
void Light::setBrightness(uint8_t _level)
{

    brightness_value = _level;

    if (color_mode == _HASSIO_ENTITY_LIGHT_COLOR_MODE_BRIGHTNESS)
    {
        analogWrite(light_pin, constrain(brightness_value, 0, 255)); // Map to PWM range
    }

    if (color_mode == _HASSIO_ENTITY_LIGHT_COLOR_MODE_COLORTEMP)
    {
        analogWrite(CCT_pins[0], constrain(cct_cool_value * brightness_value / 255.0, 0, 255));
        analogWrite(CCT_pins[1], constrain(cct_warm_value * brightness_value / 255.0, 0, 255));
    }

    if (color_mode == _HASSIO_ENTITY_LIGHT_COLOR_MODE_RGB)
    {
        analogWrite(rgb_pins[0], constrain(r_value * brightness_value / 255.0, 0, 255));
        analogWrite(rgb_pins[1], constrain(g_value * brightness_value / 255.0, 0, 255));
        analogWrite(rgb_pins[2], constrain(b_value * brightness_value / 255.0, 0, 255));
    }
}

// Set Color Temperature (mired)
void Light::setColorTemperature(int _mired)
{

    /* ---------------------------- Update Color Mode --------------------------- */
    color_mode = _HASSIO_ENTITY_LIGHT_COLOR_MODE_COLORTEMP;

    /* -------------------- Update Color Temperature in Mired ------------------- */
    color_temp = _mired;
    
    float temp_ratio = ((float)_mired - (float)min_mireds) / ((float)max_mireds - (float)min_mireds); // Map mireds to range between CCT

    cct_cool_value = (1.0 - temp_ratio) * (float)brightness_value;
    cct_warm_value =        temp_ratio  * (float)brightness_value;
    Serial.println(_mired);
    Serial.println(temp_ratio);
    Serial.println(cct_cool_value);
    Serial.println(cct_warm_value);
    
    // Clamp values to the PWM range and write to the pins
    if (rgb_pins[0] != 0)
        analogWrite(rgb_pins[0], 0);
    if (rgb_pins[1] != 0)
        analogWrite(rgb_pins[1], 0);
    if (rgb_pins[2] != 0)
        analogWrite(rgb_pins[2], 0);

    analogWrite(CCT_pins[0], cct_cool_value);
    analogWrite(CCT_pins[1], cct_warm_value);
}

// Set RGB Color in xy coordinates
void Light::setRGB(int _rgb_r, int _rgb_g, int _rgb_b)
{
    /* ---------------------------- Update Color Mode --------------------------- */
    color_mode = _HASSIO_ENTITY_LIGHT_COLOR_MODE_RGB;

    r_value = _rgb_r;
    g_value = _rgb_g;
    b_value = _rgb_b;

    if (CCT_pins[0] != 0)
        analogWrite(CCT_pins[0], 0);
    if (CCT_pins[1] != 0)
        analogWrite(CCT_pins[1], 0);

    // Clamp values to the PWM range and write to the pins
    analogWrite(rgb_pins[0], constrain(r_value * brightness_value / 255.0, 0, 255));
    analogWrite(rgb_pins[1], constrain(g_value * brightness_value / 255.0, 0, 255));
    analogWrite(rgb_pins[2], constrain(b_value * brightness_value / 255.0, 0, 255));
 
}

/* -------------------------------------------------------------------------- */
/*                                SWITCH CLASS                                */
/* -------------------------------------------------------------------------- */
Switch::Switch(String _name)
    : Entity(_name, _HASSIO_ENTITY_TYPE_SWITCH)
{
    device_class = _HASSIO_DEVICE_CLASS_SWITCH;
    payload_on = ENTITY_SWITCH_PAYLOAD_ON;
    payload_off = ENTITY_SWITCH_PAYLOAD_OFF;
}
Switch::Switch(String _name, uint8_t _pin)
    : Switch(_name)
{
    pin = _pin;
}

JsonDocument Switch::getConfigJson(JsonDocument &_entityConfig)
{
    _entityConfig[_HASSIO_ENTITY_NAME] = name;
    _entityConfig[_HASSIO_ENTITY_ENABLED_BY_DEFAULT] = enabled_by_default;
    _entityConfig[_HASSIO_ENTITY_UNIQUE_ID] = unique_id;
    _entityConfig[_HASSIO_ENTITY_STATE_TOPIC] = state_topic;
    _entityConfig[_HASSIO_ENTITY_VALUE_TEMPLATE] = stringValueTemplate(name);
    _entityConfig[_HASSIO_ENTITY_COMMAND_TOPIC] = command_topic;
    _entityConfig[_HASSIO_ENTITY_PAYLOAD_ON] = payload_on;
    _entityConfig[_HASSIO_ENTITY_PAYLOAD_OFF] = payload_off;
    _entityConfig[_HASSIO_ENTITY_DEVICE_CLASS] = device_class;
    _entityConfig[_HASSIO_ENTITY_EXPIRE_AFTER] = expire_after;

    return _entityConfig;
}

void Switch::setStatus(bool valueToSet)
{

    // Control output pin if it is defined
    if (pin != 0){
        digitalWrite(pin, valueToSet);
    }
        

    if (true == valueToSet)
        state = payload_on;
    else
        state = payload_off;
}

String Switch::getStatus()
{

    // Read Status of the output pin if it is defined
    if (pin != 0){
        setStatus(digitalRead(pin));
    }
        
    return state;
}

void Switch::hw_init(){
    if (pin != 0 ){
        pinMode(pin, OUTPUT);
    }
}

/* -------------------------------------------------------------------------- */
/*                             BINARY SENSOR CLASS                            */
/* -------------------------------------------------------------------------- */
binarySensor::binarySensor(String _name)
    : Entity(_name, _HASSIO_ENTITY_TYPE_BINARY_SENSOR)
{
}

binarySensor::binarySensor(String _name, uint8_t _pin)
    : binarySensor(_name)
{
    pin = _pin;
}

void binarySensor::hw_init(){
    pinMode(pin, INPUT);
}

JsonDocument binarySensor::getConfigJson(JsonDocument &_entityConfig)
{

    _entityConfig[_HASSIO_ENTITY_STATE_TOPIC] = state_topic;
    _entityConfig[_HASSIO_ENTITY_ENABLED_BY_DEFAULT] = enabled_by_default;
    _entityConfig[_HASSIO_ENTITY_VALUE_TEMPLATE] = stringValueTemplate(name);

    if (name != "")
        _entityConfig[_HASSIO_ENTITY_NAME] = name;
    if (unique_id != "")
        _entityConfig[_HASSIO_ENTITY_UNIQUE_ID] = unique_id;
    if (payload_on != "")
        _entityConfig[_HASSIO_ENTITY_PAYLOAD_ON] = payload_on;
    if (payload_off != "")
        _entityConfig[_HASSIO_ENTITY_PAYLOAD_OFF] = payload_off;
    if (device_class != "")
        _entityConfig[_HASSIO_ENTITY_DEVICE_CLASS] = device_class;
    if (expire_after != 0)
        _entityConfig[_HASSIO_ENTITY_EXPIRE_AFTER] = expire_after;

    return _entityConfig;
}

void binarySensor::setStatus(bool valueToSet)
{
    if (true == valueToSet)
        state = payload_on;
    else
        state = payload_off;
}

String binarySensor::getStatus()
{

    // Read Status of the output pin if it is defined
    if (pin != -1)
        setStatus(digitalRead(pin));
    return state;
}

/* -------------------------------------------------------------------------- */
/*                                SENSOR CLASS                                */
/* -------------------------------------------------------------------------- */
Sensor::Sensor(String _name)
    : Entity(_name, _HASSIO_ENTITY_TYPE_SENSOR)
{
}

Sensor::Sensor(String _name, String _deviceClass)
    : Entity(_name, _HASSIO_ENTITY_TYPE_SENSOR)
{
    device_class = _deviceClass;
}

JsonDocument Sensor::getConfigJson(JsonDocument &_entityConfig)
{

    _entityConfig[_HASSIO_ENTITY_NAME] = name;
    _entityConfig[_HASSIO_ENTITY_ENABLED_BY_DEFAULT] = enabled_by_default;
    _entityConfig[_HASSIO_ENTITY_UNIQUE_ID] = unique_id;
    _entityConfig[_HASSIO_ENTITY_STATE_TOPIC] = state_topic;
    _entityConfig[_HASSIO_ENTITY_VALUE_TEMPLATE] = stringValueTemplate(name);
    if (payload_on != "")
        _entityConfig[_HASSIO_ENTITY_PAYLOAD_ON] = payload_on;
    if (payload_off != "")
        _entityConfig[_HASSIO_ENTITY_PAYLOAD_OFF] = payload_off;
    if (device_class != "")
        _entityConfig[_HASSIO_ENTITY_DEVICE_CLASS] = device_class;
    if (expire_after != 0)
        _entityConfig[_HASSIO_ENTITY_EXPIRE_AFTER] = expire_after;
    if (suggested_display_precision != 0)
        _entityConfig[_HASSIO_ENTITY_SUGGESTED_DISPLAY_PRECISION] = suggested_display_precision;

    return _entityConfig;
}

void Sensor::setStatus(double valueToSet)
{
    state = String(valueToSet);
}
void Sensor::setStatus(const String &valueToSet)
{
    state = valueToSet;
}

/* -------------------------------------------------------------------------- */
/*                            DEVICE TRIGGER CLASS                            */
/* -------------------------------------------------------------------------- */
deviceTrigger::deviceTrigger(String _name)
    : Entity(_name, _HASSIO_ENTITY_TYPE_DEVICE_TRIGGER)
{
    trigger_type = "action";
    payload = name;
    trigger_subtype = name;
    state = name;
}

JsonDocument deviceTrigger::getConfigJson(JsonDocument &_entityConfig)
{

    _entityConfig[_HASSIO_ENTITY_AUTOMATION_TYPE] = automation_type;
    _entityConfig[_HASSIO_ENTITY_PAYLOAD] = payload;
    _entityConfig[_HASSIO_ENTITY_TOPIC] = topic;
    _entityConfig[_HASSIO_ENTITY_TYPE] = trigger_type;
    _entityConfig[_HASSIO_ENTITY_SUBTYPE] = trigger_subtype;

    return _entityConfig;
}

/* -------------------------------------------------------------------------- */
/*                                NUMBER CLASS                                */
/* -------------------------------------------------------------------------- */
Number::Number(String _name)
    : Entity(_name, _HASSIO_ENTITY_TYPE_NUMBER)
{
    device_class = _HASSIO_DEVICE_CLASS_NUMBER_NONE;
    state = "25";
}

Number::Number(String _name, String _deviceClass)
    : Entity(_name, _HASSIO_ENTITY_TYPE_NUMBER)
{
    device_class = _deviceClass;
    state = "25";
}

JsonDocument Number::getConfigJson(JsonDocument &_entityConfig){
    _entityConfig[_HASSIO_ENTITY_NAME] = name;
    _entityConfig[_HASSIO_ENTITY_ENABLED_BY_DEFAULT] = enabled_by_default;
    _entityConfig[_HASSIO_ENTITY_UNIQUE_ID] = unique_id;
    _entityConfig[_HASSIO_ENTITY_STATE_TOPIC] = state_topic;
    _entityConfig[_HASSIO_ENTITY_VALUE_TEMPLATE] = stringValueTemplate(name);
    _entityConfig[_HASSIO_ENTITY_COMMAND_TOPIC] = command_topic;
    _entityConfig[_HASSIO_ENTITY_DEVICE_CLASS] = device_class;
    _entityConfig[_HASSIO_ENTITY_PLATFORM] = _HASSIO_ENTITY_NUMBER_PLATFORM;
    _entityConfig[_HASSIO_ENTITY_NUMBER_STEP] = step;
    _entityConfig[_HASSIO_ENTITY_NUMBER_MIN] = min;
    _entityConfig[_HASSIO_ENTITY_NUMBER_MAX] = max;
    return _entityConfig;
}