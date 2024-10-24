#include "IoT_device.h"

#include <string>
#include <cstdio>   // Per snprintf
#include <cstdlib>  // Per malloc



/* -------------------------------------------------------------------------- */
/*                              HELPER FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
String stringSnakeCase(const String& entityName) {
    String output = "";

    for (char currentChar : entityName) {
        if (currentChar == ' ') {
            output += "_";  // Replace space with underscore
        } else {
            if (isUpperCase(currentChar)) {
                if (output.length() > 0 && (isLowerCase(output.charAt(output.length() - 1)) || isDigit(output.charAt(output.length() - 1)))) {
                    output += "_";  // Add underscore before uppercase letter
                }
                currentChar = toLowerCase(currentChar);  // Convert to lowercase
            }
            output += currentChar;  // Append the character
        }
    }
    
    return output;
}

static String  stringValueTemplate(const String& entityName) {
    String  prefix = "{{value_json.";
    String  suffix = "}}";

    String  output = prefix + stringSnakeCase(entityName) + suffix;

    return output;
}

/* -------------------------------------------------------------------------- */
/*                                DEVICE CLASS                                */
/* -------------------------------------------------------------------------- */
Device::Device() {
    identifiers = DEVICE_IDENTIFIER;
    manufacturer = DEVICE_MANUFACTURER;
    model = DEVICE_MODEL;
    name = DEVICE_NAME;
    sw_version = DEVICE_SW_VERS;
    serial_number = DEVICE_SERIAL_NUMBER;
    setStateTopic();
}

 void Device::setStateTopic(){
    state_topic = manufacturer +
                    "/" + model +
                    "/" + serial_number;
}
    

/* ----------------------- Add entities to the device ----------------------- */
void Device::addEntity(const std::shared_ptr<Entity>& entity) {
    
    // Correctly calling getSerialNumber() on the current Device instance
    entity->generateUniqueID(entity->getName(), this->getSerialNumber());
    
    // Update the State Topic
    entity->setStateTopic(this->getDeviceStateTopic());
    
    // Update the Command topic 
    entity->setCommandTopic();
    
    // Update the Config Topic
    entity->setConfigTopic();

    // Add the entity to the list
    entities.push_back(entity); // Storing the shared pointer
}

JsonDocument Device::getEntityStatus(){
    JsonDocument EntitiesStatus;
    
    for (size_t i = 0; i < entities.size(); ++i) {
            
        const std::shared_ptr<Entity>& entityPtr = entities[i];

        // Loop through all entities and add to the JSON object
        for (const auto& entityPtr : entities) {
            EntitiesStatus[stringSnakeCase(entityPtr->getName())] = entityPtr->getStatus();
        }
    }

    return EntitiesStatus;
}

JsonDocument Device::getEntityConfigJSON(const std::shared_ptr<Entity>& entity){
    
    JsonDocument _entityConfig;
    JsonObject _device = _entityConfig.createNestedObject("device");
    
    /* ----------------------- define device configuration ---------------------- */
    _device[_HASSIO_DEVICE_IDENTIFIERS] = identifiers;
    _device[_HASSIO_DEVICE_MANUFACTURER] = manufacturer;
    _device[_HASSIO_DEVICE_MODEL] = model;
    _device[_HASSIO_DEVICE_NAME] = name;
    _device[_HASSIO_DEVICE_SW_VERSION] = sw_version;
   
    /* ------------------------ add entity configuration ------------------------ */
    entity -> getConfigJson(_entityConfig);

    return _entityConfig;
}

String Device::getEntityConfigString(const std::shared_ptr<Entity> &entity){
    
    String entityConfig;
    
    serializeJsonPretty(getEntityConfigJSON(entity), entityConfig);
    
    return entityConfig;
}

void Device::configure(PubSubClient &_mqttClient){
    
    if (_mqttClient.connected()) {
        for (size_t i = 0; i < entities.size(); ++i) {
            
            const std::shared_ptr<Entity>& entityPtr = entities[i];

            /* ------- Loop trhough all the entities and publish the configuration ------ */
            for (const auto& entityPtr : entities) {
                _mqttClient.publish(entityPtr->getConfigTopic().c_str(), Device::getEntityConfigString(entityPtr).c_str());
                _mqttClient.subscribe(entityPtr->getCommandTopic().c_str());
            }         
        }
    }
 }

 void Device::update(PubSubClient &_mqttClient){
    String entitiesState;
    
    if (_mqttClient.connected()) {
       serializeJsonPretty(getEntityStatus(), entitiesState);
       _mqttClient.publish(state_topic.c_str(), entitiesState.c_str());
    }
 }


 


/* -------------------------------------------------------------------------- */
/*                                ENTITY CLASS                                */
/*--------------------------------------------------------------------------- */

Entity::Entity(String _name,  String _type) 
  : name(_name), type(_type) {
    
  }

JsonDocument  Entity::getConfigJson(JsonDocument& _entityConfig){
   return _entityConfig;
}

String Entity::getName(){
    return name;
}

String Entity::generateUniqueID(const String& entityName, const String& deviceId) {
    // Converti il nome dell'entità in snake case (funzione stringSnakeCase)
    String entityNameSnakeCase = stringSnakeCase(entityName);
    
    // Combina deviceId e entityNameSnakeCase
    unique_id = deviceId + "_" + entityNameSnakeCase;

    return unique_id;
}

void Entity::setStateTopic(const String& _stateTopic){
    state_topic = _stateTopic;
}
    
void Entity::setCommandTopic(){
    command_topic= state_topic +  
                        "/" + stringSnakeCase(name) + 
                        "/SET"; 
}

void Entity::setConfigTopic(){
    config_topic = "homeassistant/"+ type +"/" + unique_id +"/" + name +"/config";
}

String Entity::getConfigTopic(){
    return config_topic;
}

String Entity::getCommandTopic(){
    return command_topic;
}

/* -------------------------------------------------------------------------- */
/*                                SWITCH CLASS                                */
/* -------------------------------------------------------------------------- */
Switch::Switch(String _name) 
  : Entity(_name, _HASSIO_ENTITY_TYPE_SWITCH) {
    device_class = _HASSIO_DEVICE_CLASS_ENTITY_SWITCH;
}

void Switch::setDeviceClass(const char* _deviceClass){
    device_class = _deviceClass;
}

JsonDocument Switch::getConfigJson(JsonDocument& _entityConfig){    
    
    _entityConfig[_HASSIO_ENTITY_NAME] = name;
    _entityConfig[_HASSIO_ENTITY_ENABLED_BY_DEFAULT] = enabled_by_default;
    _entityConfig[_HASSIO_ENTITY_UNIQUE_ID] = unique_id;
    _entityConfig[_HASSIO_ENTITY_STATE_TOPIC] = state_topic;
    _entityConfig[_HASSIO_ENTITY_VALUE_TEMPLATE] = stringValueTemplate(name);
    _entityConfig[_HASSIO_ENTITY_COMMAND_TOPIC] = command_topic;
    _entityConfig[_HASSIO_ENTITY_PAYLOAD_ON] = payload_on;
    _entityConfig[_HASSIO_ENTITY_PAYLOAD_OFF] = payload_off;
    _entityConfig[_HASSIO_ENTITY_DEVICE_CLASS] = device_class;
    
    return _entityConfig;
}

void Switch::assignValueVariable(const char* externalValue){
    state = externalValue;
}

void Switch::assignValueVariable(int& externalValue){
    switchState = &externalValue;
}

 String Switch::getSwitchStatus(){
    return String(*switchState);
 }

void Switch::setStatus(bool valueToSet){
   if (true == valueToSet) state = payload_on; else state = payload_off;
 }

/* -------------------------------------------------------------------------- */
/*                                SENSOR CLASS                                */
/* -------------------------------------------------------------------------- */
