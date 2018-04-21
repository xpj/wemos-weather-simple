#include <WiFiClient.h>
#include <PubSubClient.h>
#include <utility>
#include "WeatherOutputDevice.h"
#include <BME280TG.h>

class MQTTWeatherOutputDevice : public WeatherOutputDevice {
public:
    MQTTWeatherOutputDevice(
            BME280TG *bme280,
            const char *mqttServer_m,
            uint16 mqttPort_m,
            const char *mqttUser_m,
            const char *mqttPass_m,
            const char *topic_m,
            const char *key_m) : WeatherOutputDevice(bme280) {
        mqttServer = mqttServer_m;
        mqttPort = mqttPort_m;
        mqttUser = mqttUser_m;
        mqttPass = mqttPass_m;
        topic = topic_m;
        key = key_m;

        wifiClient = new WiFiClient;
        mqttClient = new PubSubClient(mqttServer, mqttPort, *wifiClient);
    }

    void process(units_t &event280) override {
        if (isMqttConnected()) {
            mqttClient->loop();
            mqttPublishTopic(event280);
        }
    }


private:
    const char *mqttServer;
    uint16 mqttPort;
    const char *mqttUser;
    const char *mqttPass;
    const char *topic;
    const char *key;
    WiFiClient *wifiClient;
    PubSubClient *mqttClient;

    long lastReconnectAttempt = 0;

    boolean isMqttConnected() {
        if (!mqttClient->connected()) {
            Serial.println("MQTT Not Connected...");
            long now = millis();
            if (now - lastReconnectAttempt > 5000) {
                lastReconnectAttempt = now;
                if (mqttClient->connect("sws", mqttUser, mqttPass)) {
                    lastReconnectAttempt = 0;
                    Serial.println("MQTT connected");
                }
            }
        }
        return mqttClient->connected();
    }

    std::string createFieldFloat(std::string field, float value) {
        char val[10];
        dtostrf(value, 3, 2, val);
        std::string fieldString(field);
        fieldString += "=";
        fieldString += val;
        return fieldString;
    }

    std::string createBme280TopicPayload(units_t &event280) {
        std::string payload(key);
        payload += createFieldFloat("temperature", temperature(event280));
        payload += ",";
        payload += createFieldFloat("humidity", humidity(event280));
        payload += ",";
        payload += createFieldFloat("pressure", pressure(event280));
        return payload;
    }

    void mqttPublishTopic(units_t &event280) {
        mqttClient->publish(topic, createBme280TopicPayload(event280).c_str());
    }
};