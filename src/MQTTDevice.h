#ifndef MQTT_DEVICE_H
#define MQTT_DEVICE_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <utility>
#include "WeatherOutputDevice.h"

class MQTTDevice {
public:
    MQTTDevice(const char *mqttServer_m,
         uint16 mqttPort_m,
         const char *mqttUser_m,
         const char *mqttPass_m) {
        mqttServer = mqttServer_m;
        mqttPort = mqttPort_m;
        mqttUser = mqttUser_m;
        mqttPass = mqttPass_m;

        wifiClient = new WiFiClient;
        mqttClient = new PubSubClient(mqttServer, mqttPort, *wifiClient);
    }

    void mqttPublishTopic(const char *topic, const char *payload) {
        mqttClient->publish(topic, payload);
    }

    std::string createFieldFloat(std::string field, float value) {
        char val[10];
        dtostrf(value, 3, 2, val);
        std::string fieldString(field);
        fieldString += "=";
        fieldString += val;
        return fieldString;
    }

    void loop() {
        mqttClient->loop();
    }

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

private:
    const char *mqttServer;
    uint16 mqttPort;
    const char *mqttUser;
    const char *mqttPass;
    WiFiClient *wifiClient;
    PubSubClient *mqttClient;

    long lastReconnectAttempt = 0;

};

#endif //MQTT_DEVICE_H