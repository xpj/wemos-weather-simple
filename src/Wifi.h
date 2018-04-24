#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>

class Wifi {
public:
    Wifi(const char* wifiSsid, const char* wifiPass) {
        WiFi.begin(wifiSsid, wifiPass);
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
};

#endif //WIFI_H