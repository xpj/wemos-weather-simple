
#ifndef MQTTWEATHEROUTPUTDEVICE_H
#define MQTTWEATHEROUTPUTDEVICE_H

#include "WeatherOutputDevice.h"
#include "MQTTDevice.h"

class MQTTWeatherOutputDevice: public WeatherOutputDevice {
public:
    MQTTWeatherOutputDevice(BME280Device *bme280, MQ135Device *mq135, MQTTDevice *mqtt_m) : WeatherOutputDevice(bme280, mq135) {
        mqtt = mqtt_m;
    }

    void configureBME280(const char* topic, const char* key) {
        bme280_key = key;
        bme280_topic = topic;
    }
    void configureMQ135(const char* topic, const char* key) {
        mq135_key = key;
        mq135_topic = topic;
    }

    void process(Weather weatherEvent) override {
        if (mqtt->isMqttConnected()) {
            mqtt->loop();
            if (weatherEvent.bme280Connected) {
                mqtt->mqttPublishTopic(bme280_topic, createBme280TopicPayload(bme280_key, weatherEvent).c_str());
            }
            if (weatherEvent.mq135Connected) {
                mqtt->mqttPublishTopic(mq135_topic, createMq135TopicPayload(mq135_key, weatherEvent).c_str());
            }
        }
    }

private:
    MQTTDevice *mqtt;
    const char *bme280_topic;
    const char *bme280_key;
    const char *mq135_topic;
    const char *mq135_key;

    std::string createMq135TopicPayload(const char *key, Weather weatherEvent) {
        std::string payload(key);
        payload += " ";
        payload += mqtt->createFieldFloat("r0", rzero(weatherEvent));
        payload += ",";
        payload += mqtt->createFieldFloat("ratio", ratio(weatherEvent));
        payload += ",";
        payload += mqtt->createFieldFloat("co2", co2(weatherEvent));
        return payload;
    }

    std::string createBme280TopicPayload(const char *key, Weather weatherEvent) {
        std::string payload(key);
        payload += " ";
        payload += mqtt->createFieldFloat("temperature", temperature(weatherEvent));
        payload += ",";
        payload += mqtt->createFieldFloat("humidity", humidity(weatherEvent));
        payload += ",";
        payload += mqtt->createFieldFloat("pressure", pressure(weatherEvent));
        return payload;
    }
};

#endif //MQTTWEATHEROUTPUTDEVICE_H
