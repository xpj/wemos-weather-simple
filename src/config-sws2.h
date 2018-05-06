#ifndef CONFIG_SWS1_H
#define CONFIG_SWS1_H

#undef SUPPORT_OLED
#define SUPPORT_EPAPER
#define SUPPORT_MQTT
#define SUPPORT_BLYNK
#undef DEEP_SLEEP

#define MQTT_BME280_TOPIC "sensors/sws2/bme280"
#define MQTT_BH1750_TOPIC "sensors/sws2/bh1750"
#define MQTT_MQ135_TOPIC "sensors/sws2/mq135"
#define MQTT_KEY "sws2"

#endif //CONFIG_SWS1_H
