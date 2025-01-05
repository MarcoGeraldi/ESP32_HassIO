#ifndef _IOT_DEVICE_H
#define _IOT_DEVICE_H

#include <ArduinoJson.h>
#include <vector>
#include <memory>
#include <PubSubClient.h>

#include "IoT_device_config.h"

#define MQTT_MAX_PACKET_SIZE 1024

// Classe base Entity
class Entity
{
protected:
    // Type of the Entity. Can be Sensor, switch, etc.
    String type = "null";
    ;

    // Sets the class of the device, changing the device state and icon that is displayed on the frontend
    String device_class = "";

    // Flag which defines if the entity should be enabled when first added.
    bool enabled_by_default = true;

    // The encoding of the payloads received. Set to "" to disable decoding of incoming payload.
    String encoding = "";

    // The category of the entity. When set, the entity category must be diagnostic for sensors.
    String entity_category = "";

    // If set, it defines the number of seconds after the sensor’s state expires, if it’s not updated.
    // After expiry, the sensor’s state becomes unavailable. Default the sensors state never expires.
    int expire_after = 120;

    // Sends update events (which results in update of state object’s last_changed) even if the sensor’s
    // state hasn’t changed. Useful if you want to have meaningful value graphs in history or want to create
    // an automation that triggers on every incoming state message (not only when the sensor’s new state is
    // different to the current one).
    bool force_update = false;

    // Sends update events (which results in update of state object’s last_changed) even if the sensor’s
    // state hasn’t changed. Useful if you want to have meaningful value graphs in history or want to create
    // an automation that triggers on every incoming state message (not only when the sensor’s new state is
    // different to the current one).
    String icon = "";

    // The name of the entity;
    String name;

    /* The MQTT topic subscribed to receive sensor’s state. Valid states are OFF and ON. Custom OFF and ON values can be set with the payload_off and payload_on config options.*/
    String state_topic;

    // An ID that uniquely identifies this sensor. If two sensors have the same unique ID, Home Assistant
    //  will raise an exception.
    String unique_id;

    // Defines a template to extract the value. If the template throws an error, the current state will be used instead.
    String value_template;

    // The MQTT topic to publish commands to change the entity state.
    String command_topic;

    // The MQTT topic to publish the initial entity configuration,
    String config_topic;

    // Status of the entity
    String state;

    // The string that represents the online state.
    String payload_available = "online";

    // The string that represents the offline state.
    String payload_not_available = "offline";

    // The string that represents the off state. It will be compared to the message in the state_topic (see value_template for details)
    String payload_off = "OFF";

    // The string that represents the on state. It will be compared to the message in the state_topic (see value_template for details)
    String payload_on = "ON";

    // For sensors that only send on state updates (like PIRs), this variable sets a delay in seconds after which the sensor’s state will be updated back to off
    int off_delay = 0;

    // The MQTT topic subscribed to receive trigger events.
    String topic = "";

    // The type of the trigger
    String trigger_type = "";

    // to check if new trigger occured
    bool trigger_flag = false;

public:
    Entity(String _name, String _type);

    ~Entity() {}

    virtual JsonDocument getConfigJson(JsonDocument &_entityConfig);
    // virtual String setStateJson();

    // Assegna una variabile per aggiornare il valore dell'entità
    virtual void assignValueVariable(const char *externalValue) {}
    virtual void assignValueVariable(int &externalValue) {}
    // virtual void assignValueVariable(bool& externalValue){}
    // void assignValueVariable(float& externalValue);

    virtual String getStatus() { return state; }
    virtual void setStatus(const String &valueToSet) { state = valueToSet;};
    virtual void setStatus(int valueToSet) {state = String(valueToSet);};
    virtual void setStatus(bool valueToSet) {state = String(valueToSet);};
    virtual void setStatus(double valueToSet) {state = String(valueToSet);};

    virtual void hw_init(){};

    // String getStatesJson();
    String generateUniqueID(const String &entityName, const String &deviceId);
    virtual void setStateTopic(const String &_stateTopic);
    void setCommandTopic();
    void setConfigTopic(const String &_configTopic);
    void setTopic(const String &_setTopic) { topic = _setTopic; }
    String getConfigTopic();
    String getCommandTopic();
    String getName();
    String getType() { return type; }
    String getPayloadOn() { return payload_on; }
    String getPayloadOff() { return payload_off; }
    String getTriggerType() { return trigger_type; }
    String getTopic() { return topic; }
    String getStateTopic() {return state_topic;}
    bool getTriggerFlag() { return trigger_flag; }
    void setDeviceClass(const char *_deviceClass);
    void setTriggerFlag(bool _triggerFlag) { trigger_flag = _triggerFlag; }

};

// Classe dispositivo (Device)
class Device
{
private:
    String configuration_url;
    String hw_version;
    String manufacturer;
    String model;
    String model_id;
    String name;
    String serial_number;
    String suggested_area;
    String sw_version;
    String identifiers;
    String state_topic;
    std::vector<std::shared_ptr<Entity>> entities; // Stores smart pointers to entities

public:
    Device();

    void addEntity(const std::shared_ptr<Entity> &entity);

    String getSerialNumber() { return serial_number; }
    String getDeviceManufacturer() { return manufacturer; }
    String getDeviceModel() { return model; }
    String getDeviceStateTopic() { return state_topic; }

    // JsonDocument getDeviceConfig();
    JsonDocument getEntityConfigJSON(const std::shared_ptr<Entity> &entity);
    String getEntityConfigString(const std::shared_ptr<Entity> &entity);
    JsonDocument getEntityStatus();
    void setStateTopic();

    void configure(PubSubClient &_mqttClient);
    void update(PubSubClient &_mqttClient);
};

class Switch : public Entity
{
private:
    uint8_t pin = 0;

public:
    Switch(String _name);
    Switch(String _name, uint8_t _pin);

    JsonDocument getConfigJson(JsonDocument &_entityConfig) override;
    void setStatus(bool valueToSet) override;
    String getStatus() override;
    void hw_init() override;
};

class binarySensor : public Entity
{
private:
    uint8_t pin = -1;

public:
    binarySensor(String _name);
    binarySensor(String _name, uint8_t _pin);

    JsonDocument getConfigJson(JsonDocument &_entityConfig) override;
    void setStatus(bool valueToSet) override;
    String getStatus() override;
    void hw_init() override;
};

class Sensor : public Entity
{
    int suggested_display_precision = 0;

private:
    ;

public:
    Sensor(String _name);
    Sensor(String _name, String _deviceClass);

    JsonDocument getConfigJson(JsonDocument &_entityConfig) override;
    void setStatus(double valueToSet) override;
    void setStatus(const String &valueToSet) override;
};

class deviceTrigger : public Entity
{
private:
    String trigger_subtype = "";
    String automation_type = "trigger";
    String payload = "";
    String subtype = "";

public:
    deviceTrigger(String _name);

    JsonDocument getConfigJson(JsonDocument &_entityConfig) override;
};

class Light : public Entity {
    
  private:
    bool brightness;
    String schema;  
    String color_mode;
    std::vector<const char*> supported_color_modes;
    int max_mireds = 500;
    int min_mireds = 142;
    int color_temp;
    int h =0;
    int hue = 0;
    int s=0; 
    int saturation=0; 
    float x=0;
    float y=0;
    int brightness_value=0;

    float r_value=0, g_value=0, b_value=0;
    float cct_cool_value=0, cct_warm_value=0;

    uint8_t rgb_pins[3] = {0,0,0}; // RGB pins
    uint8_t CCT_pins[2] = {0,0};     // CCT pins
    uint8_t light_pin = 0; //Single color pin
 
  public:
    // Base constructor
    Light(String _name);

    // Single color light constructor
    Light(String _name, uint8_t single_pin);

    // CCT light constructor (using two separate uint8_t parameters for pins)
    Light(String _name, uint8_t _CCTCoolPin, uint8_t _CCTWarmPin);

    // RGB light constructor (using three separate uint8_t parameters for pins)
    Light(String _name, uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin);

    // CCT + RGB light constructor (using five separate uint8_t parameters for pins)
    Light(String _name, uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin, uint8_t _CCTCoolPin, uint8_t _CCTWarmPin);


    void hw_init() override;

    JsonDocument getConfigJson(JsonDocument &_entityConfig) override;
    void setStateTopic(const String &_stateTopic) override;
    void setStatus(const String &valueToSet) override;
    

    void setBrightness(uint8_t _level);
    void setColorTemperature(int _mired);
    void setRGB(int _rgb_r, int _rgb_g, int _rgb_b);

    String getStatus() override;
    
};

class Number : public Entity{
    private: ;
        double min = 20;
        double max = 50;
        double step = 1;

    public: 
        Number(String _name);
        Number(String _name, String _deviceClass);
        
        JsonDocument getConfigJson(JsonDocument &_entityConfig) override;
        
        void setMin(double _min){min = _min;};
        void setMax(double _max){max = _max;};
        void setStep(double _step){step = _step;};   
        
};

#endif