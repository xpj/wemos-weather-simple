#ifndef CONFIG_SWS1_H
#define CONFIG_SWS1_H

#define SUPPORT_OLED
#undef SUPPORT_EPAPER
#define SUPPORT_MQTT
#define SUPPORT_BLYNK
#undef DEEP_SLEEP

#define MQTT_BME280_TOPIC "sensors/sws1/bme280"
#define MQTT_MQ135_TOPIC "sensors/sws1/mq135"
#define MQTT_KEY "sws"

#endif //CONFIG_SWS1_H
