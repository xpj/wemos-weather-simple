#include <WiFiClient.h>
#include <PubSubClient.h>
#include "WeatherOutputDevice.h"

class MQTTWeatherOutputDevice: public WeatherOutputDevice {
public:
    MQTTWeatherOutputDevice(
            BME280TG *bme280,
            const char* mqttServer_m,
            uint16 mqttPort_m,
            const char* mqttUser_m,
            const char* mqttPass_m,
            const char* topic_m,
            const char* key_m,
            WiFiClient wiFiClient) : WeatherOutputDevice(bme280) {
        mqttServer = mqttServer_m;
        mqttPort = mqttPort_m;
        mqttUser = mqttUser_m;
        mqttPass = mqttPass_m;
        topic = topic_m;
        key = key_m;
        mqttClient = new PubSubClient(mqttServer, mqttPort, wiFiClient);
    }

    void mqttReconnect() {
        if (!mqttClient->connected()) {
            Serial.print("Attempting MQTT connection...");
            if (mqttClient->connect("sws", mqttUser, mqttPass)) {
                Serial.println("connected");
            } else {
                Serial.print("failed, rc=");
                Serial.println(mqttClient->state());
            }
        }
    }

    void process(const units_t &event280) override {
        if (!mqttClient->connected()) {
            mqttReconnect();
        } else {
            mqttClient->loop();
            mqttPublishTopic(event280);
        }
    }


private:
    const char* mqttServer;
    uint16 mqttPort;
    const char* mqttUser;
    const char* mqttPass;
    const char* topic;
    const char* key;
    PubSubClient *mqttClient;

    String createFieldFloat(const String &field, const float &value) {
        char val[10];
        dtostrf(value, 3, 2, val);
        String fieldString = field;
        fieldString += "=";
        fieldString += val;
        return fieldString;
    }

    const char* createBme280TopicPayload(const units_t &event280) {
        String payload;
        payload += key;
        payload += createFieldFloat("temperature", temperature(event280));
        payload += ",";
        payload += createFieldFloat("humidity", humidity(event280));
        payload += ",";
        payload += createFieldFloat("pressure", pressure(event280));
        return payload.c_str();
    }

    void mqttPublishTopic(const units_t &event280) {
        mqttClient->publish(topic, createBme280TopicPayload(event280));
    }
};