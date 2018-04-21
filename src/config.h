
#ifndef WEMOSWEATHERSIMPLE_CONFIG_H
#define WEMOSWEATHERSIMPLE_CONFIG_H

#include "secrets.h"

#define MQTT_TOPIC "sensors/sws1/bme280"
#define MQTT_KEY "sws"

#define SUPPORT_OLED
//#undef SUPPORT_OLED

#define SUPPORT_MQTT
//#undef SUPPORT_MQTT

#define SUPPORT_ADAFRUIT_IO
//#undef SUPPORT_ADAFRUIT_IO

#define SUPPORT_BLYNK
//#undef SUPPORT_BLYNK

//#define DEEP_SLEEP
#undef DEEP_SLEEP

#endif //WEMOSWEATHERSIMPLE_CONFIG_H
