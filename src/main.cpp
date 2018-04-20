#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include <utility>
#include <SSD1306init.h>
#include "BME280TG.h"
#include "ArduinoJson.h"

#include "Wifi.h"
#include "SerialWeatherOutputDevice.h"

#include "config.h"

#define LOOP_INTERVAL_SECONDS 5
#define DEEP_SLEEP_INTERVAL_SECONDS 60 // 60 seconds

#define I2C_ADDRESS 0x3C
#define SCLPIN  5
#define SDAPIN  4

BME280TG *bme280TG;

Wifi *wifi;

SerialWeatherOutputDevice *serialWeatherDevice;
#ifdef SUPPORT_OLED
#include "OledWeatherOutputDevice.h"
OledWeatherOutputDevice *oledWeatherDevice;
#endif

#ifdef SUPPORT_MQTT
#include "MQTTWeatherOutputDevice.h"
WiFiClient wiFiClient;
MQTTWeatherOutputDevice *mqttWeatherOutputDevice;
#endif

#ifdef SUPPORT_BLYNK
#include "BlynkWeatherOutputDevice.h"

BlynkWeatherOutputDevice *blynkWeatherOutputDevice;
#endif

#ifdef SUPPORT_ADAFRUIT_IO
#include "AdafruitIOWeatherOutputDevice.h"

AdafruitIOWeatherOutputDevice *adafruitIOWeatherOutputDevice;
#endif

void process() {
    units_t event280;
    bme280TG->get(&event280);

    serialWeatherDevice->process(event280);
#ifdef SUPPORT_OLED
    oledWeatherDevice->process(event280);
#endif
#ifdef SUPPORT_MQTT
    mqttWeatherOutputDevice->process(event280);
#endif
#ifdef SUPPORT_BLYNK
    blynkWeatherOutputDevice->process(event280);
#endif
#ifdef SUPPORT_ADAFRUIT_IO
    adafruitIOWeatherOutputDevice->process(event280);
#endif
}

void setup() {
    Serial.begin(9600);
    Wire.begin(SDAPIN, SCLPIN);
    Wire.setClock(400000L);

    bme280TG = new BME280TG();
    serialWeatherDevice = new SerialWeatherOutputDevice(bme280TG);
    serialWeatherDevice->hello();
#ifdef SUPPORT_OLED
    oledWeatherDevice = new OledWeatherOutputDevice(bme280TG, I2C_ADDRESS);
    oledWeatherDevice->hello();
#endif

    wifi = new Wifi(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
#ifdef SUPPORT_MQTT
    mqttWeatherOutputDevice = new MQTTWeatherOutputDevice(
            bme280TG,
            SECRET_MQTT_SERVER,
            1883,
            SECRET_MQTT_USERNAME,
            SECRET_MQTT_PASSWORD,
            "sensors/sws1/bme280",
            "sws",
            wiFiClient);
#endif

#ifdef SUPPORT_BLYNK
    blynkWeatherOutputDevice = new BlynkWeatherOutputDevice(bme280TG, SECRET_BLYNK_TOKEN);
#endif

#ifdef SUPPORT_ADAFRUIT_IO
    adafruitIOWeatherOutputDevice = new AdafruitIOWeatherOutputDevice(bme280TG, AIO_GROUP, SECRET_AIO_USERNAME, SECRET_AIO_TOKEN, SECRET_WIFI_SSID, SECRET_WIFI_PASS);
#endif

#ifdef DEEP_SLEEP
    pinMode(D0, WAKEUP_PULLUP);
    Serial.println("Deep sleep mode");
    delay(500);
    process();
    ESP.deepSleep(DEEP_SLEEP_INTERVAL_SECONDS * 1000000);
#else
    delay(LOOP_INTERVAL_SECONDS * 1000);
#endif
}

void loop() {
#ifndef DEEP_SLEEP
    process();
    delay(LOOP_INTERVAL_SECONDS * 1000);
#endif
}