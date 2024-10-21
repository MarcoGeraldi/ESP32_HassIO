#ifndef _IOT_DEVICE_H
#define _IOT_DEVICE_H

#include <ArduinoJson.h>
#include <vector>
#include <memory>
#include <PubSubClient.h>

// Classe base Entity
class Entity
{
protected:
    // Sets the class of the device, changing the device state and icon that is displayed on the frontend
    String device_class = "null";

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

    String getStatus() { return state; }
    virtual void setStatus(int valueToSet) {};
    virtual void setStatus(bool valueToSet) {};

    // String getStatesJson();
    String generateUniqueID(const String &entityName, const String &deviceId);
    void setStateTopic(const String &_stateTopic);
    void setCommandTopic();
    void setConfigTopic();
    String getConfigTopic();
    String getCommandTopic();
    String getName();
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

    String getSerialNumber()        {return serial_number;}
    String getDeviceManufacturer()  {return manufacturer;}
    String getDeviceModel()         {return model;}
    String getDeviceStateTopic()    {return state_topic;}

    // JsonDocument getDeviceConfig();
    JsonDocument getEntityConfigJSON(const std::shared_ptr<Entity> &entity);
    String getEntityConfigString    (const std::shared_ptr<Entity> &entity);
    JsonDocument getEntityStatus();
    void setStateTopic();
    
    void configure(PubSubClient &_mqttClient);
    void update(PubSubClient &_mqttClient);

};

class Switch : public Entity
{
private:
    String payload_off = "OFF";
    String payload_on = "ON";

    int *switchState;

public:
    Switch(String _name);

    void assignValueVariable(const char *externalValue);
    void assignValueVariable(int &externalValue);

    JsonDocument getConfigJson(JsonDocument &_entityConfig) override;
    void setStatus(bool valueToSet) override;

    String getSwitchStatus();
    String getPayloadOn() {return payload_on;}
    String getPayloadOff(){return payload_off;}
};

class Sensor : public Entity
{
private:
    ;

public:
    ;
};

/*class Light : public Entity {
  private:
    int *brightness;

  public:
    Light(String _name);

    void assignBrightness(int &var);

    //JsonDocument getConfigJson() override;
};*/

#endif