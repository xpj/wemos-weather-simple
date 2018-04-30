
#ifndef WEMOSWEATHERSIMPLE_CONFIG_H
#define WEMOSWEATHERSIMPLE_CONFIG_H

#include "secrets.h"

//#define SUPPORT_OLED
#undef SUPPORT_OLED

#define SUPPORT_EPAPER
//#undef SUPPORT_EPAPER
#ifdef SUPPORT_EPAPER
//another instance of SWS
#define MQTT_BME280_TOPIC "sensors/sws2/bme280"
#define MQTT_MQ135_TOPIC "sensors/sws2/mq135"
#define MQTT_KEY "sws2"
#endif

#define SUPPORT_MQTT
//#undef SUPPORT_MQTT

#define SUPPORT_BLYNK
//#undef SUPPORT_BLYNK

//#define DEEP_SLEEP
#undef DEEP_SLEEP

#define MQ135_PIN  A0
#define OLED_I2C_ADDRESS 0x3C
#define I2C_SCL_PIN  5
#define I2C_SDA_PIN  4

#define LOOP_INTERVAL_SECONDS 10
#define DEEP_SLEEP_INTERVAL_SECONDS 60

#ifndef MQTT_KEY
#define MQTT_BME280_TOPIC "sensors/sws1/bme280"
#define MQTT_MQ135_TOPIC "sensors/sws1/mq135"
#define MQTT_KEY "sws"
#endif

#endif //WEMOSWEATHERSIMPLE_CONFIG_H
