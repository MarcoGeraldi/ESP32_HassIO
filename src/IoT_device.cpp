#include "IoT_device.h"
#include "IoT_device_config.h"

#include <string>
#include <cstdio>   // Per snprintf
#include <cstdlib>  // Per malloc


/* -------------------------------------------------------------------------- */
/*                              HELPER FUNCTIONS                              */
/* -------------------------------------------------------------------------- */
String stringSnakeCase(const std::string& entityName) {
    String output;
    output.reserve(entityName.length());  // Pre-alloca spazio per ottimizzare

    for (char ch : entityName) {
        if (ch == ' ') {
            output += '_';
        } else {
            output += std::tolower(static_cast<unsigned char>(ch));
        }
    }

    return output;
}

static String  stringValueTemplate(const std::string& entityName) {
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

// Aggiungi entità al dispositivo
void Device::addEntity(const std::shared_ptr<Entity>& entity) {
    entities.push_back(entity); // Storing the smart pointer
}

String Device::getSerialNumber(){
    return serial_number;
}

/* -------------------------------------------------------------------------- */
/*                                ENTITY CLASS                                */
/* -----String Entity::generateUniqueID(const String& entityName, const String& deviceId) {
    // Converti il nome dell'entità in snake case (funzione stringSnakeCase)
    String entityNameSnakeCase = stringSnakeCase(entityName.c_str());
    
    // Combina deviceId e entityNameSnakeCase
    unique_id = deviceId + "_" + entityNameSnakeCase;

    return unique_id;
}--------------------------------------------------------------------- */
Entity::Entity(String _name,  String _type) 
  : name(_name), device_class(_type) {

}

JsonDocument  Entity::getConfigJson(JsonDocument _entityConfig){
   return _entityConfig;
}

String Entity::getName(){
    return name;
}

String Entity::generateUniqueID(const String& entityName, const String& deviceId) {
    // Converti il nome dell'entità in snake case (funzione stringSnakeCase)
    String entityNameSnakeCase = stringSnakeCase(entityName.c_str());
    
    // Combina deviceId e entityNameSnakeCase
    unique_id = deviceId + "_" + entityNameSnakeCase;

    return unique_id;
}

/* -------------------------------------------------------------------------- */
/*                                SWITCH CLASS                                */
/* -------------------------------------------------------------------------- */
Switch::Switch(String _name) 
  : Entity(_name, "switch") {
}

JsonDocument Switch::getConfigJson(JsonDocument _entityConfig){    
    
    _entityConfig[_HASSIO_ENTITY_ENABLED_BY_DEFAULT] = enabled_by_default;
    _entityConfig[_HASSIO_ENTITY_UNIQUE_ID] = unique_id;
    _entityConfig[_HASSIO_ENTITY_STATE_TOPIC] = state_topic;
    _entityConfig[_HASSIO_ENTITY_VALUE_TEMPLATE] = stringValueTemplate(std::string(name.c_str()));
    _entityConfig[_HASSIO_ENTITY_COMMAND_TOPIC] = command_topic;
    _entityConfig[_HASSIO_ENTITY_PAYLOAD_ON] = payload_on;
    _entityConfig[_HASSIO_ENTITY_PAYLOAD_OFF] = payload_off;
    
    return _entityConfig;
}

void Switch::assignValueVariable(const char* externalValue){
    state = externalValue;
}

/*void Switch::assignValueVariable(bool& externalValue){
    if (externalValue==true) state="ON"; else state="OFF"; 
}*/

void Switch::assignValueVariable(int& externalValue){
    switchState = &externalValue;
}

 String Switch::getSwitchStatus(){
    return String(*switchState);
 }

 void Switch::setStatus(int valueToSet){
    state = String(valueToSet);
 }

/*void Switch::assignValueVariable(int& externalValue){
    if (externalValue==true) state="ON"; else state="OFF"; 
}*/

