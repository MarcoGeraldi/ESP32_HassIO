#include "IoT_device.h"
#include "IoT_device_config.h"

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
}

String Device::getSerialNumber(){
    return serial_number;
}

String Device::getDeviceManufacturer(){
    return manufacturer;
}

String Device::getDeviceModel(){
    return model;
}

// Aggiungi entità al dispositivo
void Device::addEntity(const std::shared_ptr<Entity>& entity) {
    
    // Correctly calling getSerialNumber() on the current Device instance
    entity->generateUniqueID(entity->getName(), this->getSerialNumber());
    
    // Pass the current device instance to setStateTopic
    entity->setStateTopic(this->getDeviceManufacturer(), this->getDeviceModel(), this->getSerialNumber());
    
    // Pass the current device instance to setCommandTopic
    entity->setCommandTopic(this->getDeviceManufacturer(), this->getDeviceModel(), this->getSerialNumber());
    
    // Add the entity to the list
    entities.push_back(entity); // Storing the shared pointer
}

JsonDocument Device::getEntityStatus(){
    JsonDocument EntitiesStatus;
    
    for (size_t i = 0; i < entities.size(); ++i) {
            
        const std::shared_ptr<Entity>& entityPtr = entities[i];

        // Loop through all entities and add to the JSON object
        for (const auto& entityPtr : entities) {
            EntitiesStatus[entityPtr->getName()] = entityPtr->getStatus();
        }
    }

    return EntitiesStatus;
}

JsonDocument Device::getEntityConfig(const std::shared_ptr<Entity>& entity){
    
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
/* -------------------------------------------------------------------------- */
/*                                ENTITY CLASS                                */
/*--------------------------------------------------------------------------- */

Entity::Entity(String _name,  String _type) 
  : name(_name), device_class(_type) {
    
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

void Entity::setStateTopic(const String& deviceManufacturer, const String& deviceModel , const String& deviceSerialNumber){
    state_topic = deviceManufacturer +
                    "/" + deviceModel +
                    "/" + deviceSerialNumber;
}
    
void Entity::setCommandTopic(const String& deviceManufacturer, const String& deviceModel , const String& deviceSerialNumber){
    command_topic= deviceManufacturer + 
                        "/" + deviceModel + 
                        "/" + deviceSerialNumber + 
                        "/" + stringSnakeCase(name) + 
                        "/SET"; 
}

/* -------------------------------------------------------------------------- */
/*                                SWITCH CLASS                                */
/* -------------------------------------------------------------------------- */
Switch::Switch(String _name) 
  : Entity(_name, "switch") {
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
