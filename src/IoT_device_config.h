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
#define _HASSIO_ENTITY_SUGGESTED_DISPLAY_PRECISION "suggested_display_precision" // The number of decimals which should be used in the sensor’s state after rounding.
#define _HASSIO_ENTITY_AUTOMATION_TYPE "automation_type"                         // The type of automation, must be ‘trigger’.
#define _HASSIO_ENTITY_PAYLOAD "payload"                                         // Optional payload to match the payload being sent over the topic.
#define _HASSIO_ENTITY_TOPIC "topic"                                             // The MQTT topic subscribed to receive trigger events.
#define _HASSIO_ENTITY_TYPE "type"                                               // The type of the trigger
#define _HASSIO_ENTITY_SUBTYPE "subtype"                                         // The subtype of the trigger

/* ----------------------- Light Entity configuration ----------------------- */
#define _HASSIO_ENTITY_LIGHT_SCHEMA_DEFAULT "default"
#define _HASSIO_ENTITY_LIGHT_SCHEMA_JSON "json"
#define _HASSIO_ENTITY_LIGHT_SCHEMA_TEMPLATE "template"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_ONOFF "onoff"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_BRIGHTNESS "brightness"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_COLORTEMP "color_temp"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_HS "hs"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_XY "xy"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_RGB "rgb"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_RGBW "rgbw"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_RGBWW "rgbww"
#define _HASSIO_ENTITY_LIGHT_COLOR_MODE_WHITE "white"

/* -------------------------------------------------------------------------- */
#define ENTITY_CATEGORY_DIAGNOSTIC "diagnostic"
#define _HASSIO_ENTITY_TYPE_ALARM_CONTROL_PANEL "alarm_control_panel"
#define _HASSIO_ENTITY_TYPE_BINARY_SENSOR "binary_sensor"
#define _HASSIO_ENTITY_TYPE_BUTTON "button"
#define _HASSIO_ENTITY_TYPE_CAMERA "camera"
#define _HASSIO_ENTITY_TYPE_COVER "cover"
#define _HASSIO_ENTITY_TYPE_DEVICE_TRACKER "device_tracker"
#define _HASSIO_ENTITY_TYPE_DEVICE_TRIGGER "device_automation"
#define _HASSIO_ENTITY_TYPE_EVENT "event"
#define _HASSIO_ENTITY_TYPE_FAN "fan"
#define _HASSIO_ENTITY_TYPE_HUMIDIFIER "humidifier"
#define _HASSIO_ENTITY_TYPE_IMAGE "image"
#define _HASSIO_ENTITY_TYPE_CLIMATE "climate"
#define _HASSIO_ENTITY_TYPE_LAWN_MOWER "lawn_mower"
#define _HASSIO_ENTITY_TYPE_LIGHT "light"
#define _HASSIO_ENTITY_TYPE_LOCK "lock"
#define _HASSIO_ENTITY_TYPE_NOTIFY "notify"
#define _HASSIO_ENTITY_TYPE_NUMBER "number"
#define _HASSIO_ENTITY_TYPE_SCENE "scene"
#define _HASSIO_ENTITY_TYPE_SELECT "select"
#define _HASSIO_ENTITY_TYPE_SENSOR "sensor"
#define _HASSIO_ENTITY_TYPE_SIREN "siren"
#define _HASSIO_ENTITY_TYPE_SWITCH "switch"
#define _HASSIO_ENTITY_TYPE_UPDATE "update"
#define _HASSIO_ENTITY_TYPE_TAG_SCANNER "tag_scanner"
#define _HASSIO_ENTITY_TYPE_TEXT "text"
#define _HASSIO_ENTITY_TYPE_VACUUM "vacuum"
#define _HASSIO_ENTITY_TYPE_VALVE "valve"
#define _HASSIO_ENTITY_TYPE_WATER_HEATER "water_heater"

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

/* ----------------------- Device Class: Entity SWITCH ---------------------- */
/* - https://www.home-assistant.io/integrations/homeassistant/#device-class - */

#define _HASSIO_DEVICE_CLASS_SWITCH "switch"
#define _HASSIO_DEVICE_CLASS_SWITCH_OUTLET "outlet"

/* ----------------------- Device Class: Binary Sensor ---------------------- */
/* - https://www.home-assistant.io/integrations/binary_sensor/#device-class - */
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_NONE "none"                         // Generic on/off. This is the default and doesn’t need to be set.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_BATTERY "battery"                   // on means low, off means normal.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_BATTERY_CHARGING "battery_charging" // on means charging, off means not charging.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_CARBON_MONOXIDE "carbon_monoxide"   // on means carbon monoxide detected, off means no carbon monoxide (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_COLD "cold"                         // on means cold, off means normal.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_CONNECTIVITY "connectivity"         // on means connected, off means disconnected.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_DOOR "door"                         // on means open, off means closed.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_GARAGE_DOOR "garage_door"           // on means open, off means closed.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_GAS "gas"                           // on means gas detected, off means no gas (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_HEAT "heat"                         // on means hot, off means normal.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_LIGHT "light"                       // on means light detected, off means no light.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_LOCK "lock"                         // on means open (unlocked), off means closed (locked).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_MOISTURE "moisture"                 // on means moisture detected (wet), off means no moisture (dry).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_MOTION "motion"                     // on means motion detected, off means no motion (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_MOVING "moving"                     // on means moving, off means not moving (stopped).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_OCCUPANCY "occupancy"               // on means occupied (detected), off means not occupied (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_OPENING "opening"                   // on means open, off means closed.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_PLUG "plug"                         // on means device is plugged in, off means device is unplugged.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_POWER "power"                       // on means power detected, off means no power.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_PRESENCE "presence"                 // on means home, off means away.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_PROBLEM "problem"                   // on means problem detected, off means no problem (OK).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_RUNNING "running"                   // on means running, off means not running.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_SAFETY "safety"                     // on means unsafe, off means safe.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_SMOKE "smoke"                       // on means smoke detected, off means no smoke (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_SOUND "sound"                       // on means sound detected, off means no sound (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_TAMPER "tamper"                     // on means tampering detected, off means no tampering (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_UPDATE "update"                     // on means update available, off means up-to-date.
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_VIBRATION "vibration"               // on means vibration detected, off means no vibration (clear).
#define _HASSIO_DEVICE_CLASS_BINARY_SENSOR_WINDOW "window"                     // on means open, off means closed.

/* -------------------------- Device Class: Sensor -------------------------- */
/* ----- https://www.home-assistant.io/integrations/sensor#device-class ----- */
#define _HASSIO_DEVICE_CLASS_SENSOR_NONE ""                                                             // Generic sensor. This is the default and doesn’t need to be set.
#define _HASSIO_DEVICE_CLASS_SENSOR_APPARENT_POWER "apparent_power"                                     // Apparent power in VA.
#define _HASSIO_DEVICE_CLASS_SENSOR_AQI "aqi"                                                           // Air Quality Index (unitless).
#define _HASSIO_DEVICE_CLASS_SENSOR_ATMOSPHERIC_PRESSURE "atmospheric_pressure"                         // Atmospheric pressure in cbar, bar, hPa, mmHg, inHg, kPa, mbar, Pa or psi.
#define _HASSIO_DEVICE_CLASS_SENSOR_BATTERY "battery"                                                   // Percentage of battery that is left in %.
#define _HASSIO_DEVICE_CLASS_SENSOR_CARBON_DIOXIDE "carbon_dioxide"                                     // Carbon Dioxide in CO2 (Smoke) in ppm.
#define _HASSIO_DEVICE_CLASS_SENSOR_CARBON_MONOXIDE "carbon_monoxide"                                   // Carbon Monoxide in CO (Gas CNG/LPG) in ppm.
#define _HASSIO_DEVICE_CLASS_SENSOR_CURRENT "current"                                                   // Current in A, mA.
#define _HASSIO_DEVICE_CLASS_SENSOR_DATA_RATE "data_rate"                                               // Data rate in bit/s, kbit/s, Mbit/s, Gbit/s, B/s, kB/s, MB/s, GB/s, KiB/s, MiB/s or GiB/s.
#define _HASSIO_DEVICE_CLASS_SENSOR_DATA_SIZE "data_size"                                               // Data size in bit, kbit, Mbit, Gbit, B, kB, MB, GB, TB, PB, EB, ZB, YB, KiB, MiB, GiB, TiB, PiB, EiB, ZiB or YiB.
#define _HASSIO_DEVICE_CLASS_SENSOR_DATE "date"                                                         // Date string (ISO 8601).
#define _HASSIO_DEVICE_CLASS_SENSOR_DISTANCE "distance"                                                 // Generic distance in km, m, cm, mm, mi, yd, or in.
#define _HASSIO_DEVICE_CLASS_SENSOR_DURATION "duration"                                                 // Duration in d, h, min, or s.
#define _HASSIO_DEVICE_CLASS_SENSOR_ENERGY "energy"                                                     // Energy in J, kJ, MJ, GJ, Wh, kWh, MWh, cal, kcal, Mcal, or Gcal.
#define _HASSIO_DEVICE_CLASS_SENSOR_ENERGY_STORAGE "energy_storage"                                     // Stored energy in J, kJ, MJ, GJ, Wh, kWh, MWh, cal, kcal, Mcal, or Gcal.
#define _HASSIO_DEVICE_CLASS_SENSOR_ENUM "enum"                                                         // Has a limited set of (non-numeric) states.
#define _HASSIO_DEVICE_CLASS_SENSOR_FREQUENCY "frequency"                                               // Frequency in Hz, kHz, MHz, or GHz.
#define _HASSIO_DEVICE_CLASS_SENSOR_GAS "gas"                                                           // Gas volume in m³, ft³ or CCF.
#define _HASSIO_DEVICE_CLASS_SENSOR_HUMIDITY "humidity"                                                 // Percentage of humidity in the air in %.
#define _HASSIO_DEVICE_CLASS_SENSOR_ILLUMINANCE "illuminance"                                           // The current light level in lx.
#define _HASSIO_DEVICE_CLASS_SENSOR_IRRADIANCE "irradiance"                                             // Irradiance in W/m² or BTU/(h⋅ft²).
#define _HASSIO_DEVICE_CLASS_SENSOR_MOISTURE "moisture"                                                 // Percentage of water in a substance in %.
#define _HASSIO_DEVICE_CLASS_SENSOR_MONETARY "monetary"                                                 // The monetary value (ISO 4217).
#define _HASSIO_DEVICE_CLASS_SENSOR_NITROGEN_DIOXIDE "nitrogen_dioxide"                                 // Concentration of Nitrogen Dioxide in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_NITROGEN_MONOXIDE "nitrogen_monoxide"                               // Concentration of Nitrogen Monoxide in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_NITROUS_OXIDE "nitrous_oxide"                                       // Concentration of Nitrous Oxide in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_OZONE "ozone"                                                       // Concentration of Ozone in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_PH "ph"                                                             // Potential hydrogen (pH) value of a water solution.
#define _HASSIO_DEVICE_CLASS_SENSOR_PM1 "pm1"                                                           // Concentration of particulate matter less than 1 micrometer in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_PM25 "pm25"                                                         // Concentration of particulate matter less than 2.5 micrometers in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_PM10 "pm10"                                                         // Concentration of particulate matter less than 10 micrometers in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_POWER_FACTOR "power_factor"                                         // Power factor (unitless), unit may be None or %.
#define _HASSIO_DEVICE_CLASS_SENSOR_POWER "power"                                                       // Power in W or kW.
#define _HASSIO_DEVICE_CLASS_SENSOR_PRECIPITATION "precipitation"                                       // Accumulated precipitation in cm, in or mm.
#define _HASSIO_DEVICE_CLASS_SENSOR_PRECIPITATION_INTENSITY "precipitation_intensity"                   // Precipitation intensity in in/d, in/h, mm/d or mm/h.
#define _HASSIO_DEVICE_CLASS_SENSOR_PRESSURE "pressure"                                                 // Pressure in Pa, kPa, hPa, bar, cbar, mbar, mmHg, inHg or psi.
#define _HASSIO_DEVICE_CLASS_SENSOR_REACTIVE_POWER "reactive_power"                                     // Reactive power in var.
#define _HASSIO_DEVICE_CLASS_SENSOR_SIGNAL_STRENGTH "signal_strength"                                   // Signal strength in dB or dBm.
#define _HASSIO_DEVICE_CLASS_SENSOR_SOUND_PRESSURE "sound_pressure"                                     // Sound pressure in dB or dBA.
#define _HASSIO_DEVICE_CLASS_SENSOR_SPEED "speed"                                                       // Generic speed in ft/s, in/d, in/h, in/s, km/h, kn, m/s, mph, mm/d, or mm/s.
#define _HASSIO_DEVICE_CLASS_SENSOR_SULPHUR_DIOXIDE "sulphur_dioxide"                                   // Concentration of sulphur dioxide in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_TEMPERATURE "temperature"                                           // Temperature in °C, °F or K.
#define _HASSIO_DEVICE_CLASS_SENSOR_TIMESTAMP "timestamp"                                               // Datetime object or timestamp string (ISO 8601).
#define _HASSIO_DEVICE_CLASS_SENSOR_VOLATILE_ORGANIC_COMPOUNDS "volatile_organic_compounds"             // Concentration of volatile organic compounds in µg/m³.
#define _HASSIO_DEVICE_CLASS_SENSOR_VOLATILE_ORGANIC_COMPOUNDS_PARTS "volatile_organic_compounds_parts" // Ratio of volatile organic compounds in ppm or ppb.
#define _HASSIO_DEVICE_CLASS_SENSOR_VOLTAGE "voltage"                                                   // Voltage in V, mV.
#define _HASSIO_DEVICE_CLASS_SENSOR_VOLUME "volume"                                                     // Generic volume in L, mL, gal, fl. oz., m³, ft³, or CCF.
#define _HASSIO_DEVICE_CLASS_SENSOR_VOLUME_FLOW_RATE "volume_flow_rate"                                 // Volume flow rate in m³/h, ft³/min, L/min, gal/min.
#define _HASSIO_DEVICE_CLASS_SENSOR_VOLUME_STORAGE "volume_storage"                                     // Generic stored volume in L, mL, gal, fl. oz., m³, ft³, or CCF.
#define _HASSIO_DEVICE_CLASS_SENSOR_WATER "water"                                                       // Water consumption in L, gal, m³, ft³, or CCF.
#define _HASSIO_DEVICE_CLASS_SENSOR_WEIGHT "weight"                                                     // Generic mass in kg, g, mg, µg, oz, lb, or st.
#define _HASSIO_DEVICE_CLASS_SENSOR_WIND_SPEED "wind_speed"                                             // Wind speed in Beaufort, ft/s, km/h, kn, m/s, or mph.



#endif