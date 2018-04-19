#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>

class Wifi {
public:
    Wifi(const char* wifiSsid, const char* wifiPass) {
        WiFi.begin(wifiSsid, wifiPass);
        while(!WiFi.isConnected()) {
            Serial.println("Wifi wait for connect");
            delay(500);
        }
        status();
    }

    WiFiClient& getWiFiClient() {
        return wiFiClient;
    }

    bool reconnect() {
        if (!WiFi.isConnected()) {
            WiFi.reconnect();
            status();
        }
    }

    void status() {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }

private:
    WiFiClient wiFiClient;
};

#endif //WIFI_H