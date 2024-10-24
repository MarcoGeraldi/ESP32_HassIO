#ifndef IoT_device_config_H
#define IoT_device_config_H

/* -------------------------------------------------------------------------- */
/*                             HASSIO DEFINITIONS                             */
/* --------- https://www.home-assistant.io/integrations/sensor.mqtt/ -------- */
/* -------------------------------------------------------------------------- */

/* -------------------------- Configuration Strings ------------------------- */
#define _HASSIO_AVAILABILITY_PAYLOAD_AVAILABLE "payload_available"
#define _HASSIO_AVAILABILITY_PAYLOAD_NOT_AVAILABLE "payload_not_available"
#define _HASSIO_AVAILABILITY_TOPIC "topic"
#define _HASSIO_AVAILABILITY_VALUE_TEMPLATE "value_template"
#define _HASSIO_ENTITY_AVAILABILITY_MODE "availability_mode"
#define _HASSIO_ENTITY_AVAILABILITY_TEMPLATE "availability_template"
#define _HASSIO_ENTITY_AVAILABILITY_TOPIC "availability_topic"
#define _HASSIO_DEVICE_CONFIGURATION_URL "configuration_url"
#define _HASSIO_DEVICE_CONNECTIONS "connections"
#define _HASSIO_DEVICE_HW_VERSION "hw_version"
#define _HASSIO_DEVICE_IDENTIFIERS "identifiers"
#define _HASSIO_DEVICE_MANUFACTURER "manufacturer"
#define _HASSIO_DEVICE_MODEL "model"
#define _HASSIO_DEVICE_MODEL_ID "model_id"
#define _HASSIO_DEVICE_NAME "name"
#define _HASSIO_DEVICE_SERIAL_NUMBER "serial_number"
#define _HASSIO_DEVICE_SUGGESTED_AREA "suggested_area"
#define _HASSIO_DEVICE_SW_VERSION "sw_version"
#define _HASSIO_DEVICE_VIA_DEVICE "via_device"
#define _HASSIO_ENTITY_DEVICE_CLASS "device_class"
#define _HASSIO_ENTITY_ENABLED_BY_DEFAULT "enabled_by_default"
#define _HASSIO_ENTITY_ENCODING "encoding"
#define _HASSIO_ENTITY_CATEGORY "entity_category"
#define _HASSIO_ENTITY_EXPIRE_AFTER "expire_after"
#define _HASSIO_ENTITY_FORCE_UPDATE "force_update"
#define _HASSIO_ENTITY_ICON "icon"
#define _HASSIO_ENTITY_JSON_ATTRIBUTES_TEMPLATE "json_attributes_template"
#define _HASSIO_ENTITY_JASON_ATTRIBUTES_TOPIC "json_attributes_topic"
#define _HASSIO_ENTITY_LAST_RESET_VALUE_TEMPLATE "last_reset_value_template"
#define _HASSIO_ENTITY_NAME "name"
#define _HASSIO_ENTITY_OBJECT_ID "object_id"
#define _HASSIO_ENTITY_OPTIONS "options"
#define _HASSIO_ENTITY_PAYLOAD_AVAILABLE "payload_available"
#define _HASSIO_ENTITY_PAYLOAD_NOT_AVAILABLE "payload_not_available"
#define _HASSIO_ENTITY_SUGGESTED_DISPLAY_PRECISION "suggested_display_precision"
#define _HASSIO_ENTITY_QOS "qos"
#define _HASSIO_ENTITY_STATE_CLASS "state_class"
#define _HASSIO_ENTITY_STATE_TOPIC "state_topic"
#define _HASSIO_ENTITY_UNIQUE_ID "unique_id"
#define _HASSIO_ENTITY_UNIT_OF_MEASUREMENT "unit_of_measurement"
#define _HASSIO_ENTITY_VALUE_TEMPLATE "value_template"
#define _HASSIO_ENTITY_COMMAND_TOPIC "command_topic"
#define _HASSIO_ENTITY_PAYLOAD_ON "payload_on"
#define _HASSIO_ENTITY_PAYLOAD_OFF "payload_off"

/* ----------------------- Device Class: Entity SWITCH ---------------------- */
#define _HASSIO_DEVICE_CLASS_ENTITY_SWITCH "switch"
#define _HASSIO_DEVICE_CLASS_ENTITY_SWITCH_OUTLET "outlet"

/* -------------------------------------------------------------------------- */
#define ENTITY_CATEGORY_DIAGNOSTIC "diagnostic"
#define _HASSIO_ENTITY_TYPE_SWITCH "switch"
#define ENTITY_TYPE_SENSOR "sensor"

#define ENTITY_SWITCH_PAYLOAD_ON "ON"
#define ENTITY_SWITCH_PAYLOAD_OFF "OFF"

/* -------------------------------------------------------------------------- */
/*                             DEVICE DEFINITIONS                             */
/* -------------------------------------------------------------------------- */
#define DEVICE_IDENTIFIER "deviceIdentifier"
#define DEVICE_MANUFACTURER "deviceManufacturer"
#define DEVICE_MODEL "deviceModel"
#define DEVICE_NAME "deviceName"
#define DEVICE_SERIAL_NUMBER "0x13124142"
#define DEVICE_SW_VERS "1.2.3"

#endif