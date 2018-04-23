
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

    void processBME280(BME280Device::units_t &event) override {
        if (mqtt->isMqttConnected()) {
            mqtt->loop();
            mqtt->mqttPublishTopic(bme280_topic, createBme280TopicPayload(bme280_key, event).c_str());
        }
    }

    void processMQ135(MQ135Device::mq_t &mq135) override {
        if (mqtt->isMqttConnected()) {
            mqtt->loop();
            mqtt->mqttPublishTopic(mq135_topic, createMq135TopicPayload(mq135_key, mq135).c_str());
        }
    }
private:
    MQTTDevice *mqtt;
    const char *bme280_topic;
    const char *bme280_key;
    const char *mq135_topic;
    const char *mq135_key;

    std::string createMq135TopicPayload(const char *key, MQ135Device::mq_t &mq135) {
        std::string payload(key);
        payload += " ";
        payload += mqtt->createFieldFloat("r0", rzero(mq135));
        payload += ",";
        payload += mqtt->createFieldFloat("ratio", ratio(mq135));
        payload += ",";
        payload += mqtt->createFieldFloat("co2", co2(mq135));
        return payload;
    }

    std::string createBme280TopicPayload(const char *key, BME280Device::units_t &event280) {
        std::string payload(key);
        payload += " ";
        payload += mqtt->createFieldFloat("temperature", temperature(event280));
        payload += ",";
        payload += mqtt->createFieldFloat("humidity", humidity(event280));
        payload += ",";
        payload += mqtt->createFieldFloat("pressure", pressure(event280));
        return payload;
    }
};

#endif //MQTTWEATHEROUTPUTDEVICE_H
