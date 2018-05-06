#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include <utility>
#include <SSD1306init.h>
#include "config.h"

#include "ArduinoJson.h"

#include "Wifi.h"
#include "device/I2CScanner.h"

#include "device/output/SerialWeatherOutputDevice.h"
#include "device/sensor/MQ135Device.h"
#include "device/output/OutputDevices.h"
#include "device/sensor/BH1750Device.h"

BME280Device *bme280Device;
MQ135Device *mq135Device;
BH1750Device *bh1750Device;

I2CScanner *i2cScanner;
Wifi *wifi;

OutputDevices *outputDevices;

SerialWeatherOutputDevice *serialWeatherDevice;

#ifdef SUPPORT_OLED
#include "device/output/OledWeatherOutputDevice.h"
OledWeatherOutputDevice *oledWeatherDevice;
#endif

#ifdef SUPPORT_EPAPER
#include "device/EPaperWeatherOutputDevice.h"
EPaperWeatherOutputDevice *epaperWeatherDevice;
#endif

#ifdef SUPPORT_MQTT
#include "device/output/MQTTDevice.h"
#include "device/output/MQTTWeatherOutputDevice.h"
MQTTDevice *mqttDevice;
MQTTWeatherOutputDevice *mqttWeatherDevice;
#endif

#ifdef SUPPORT_BLYNK
#include "device/output/BlynkWeatherOutputDevice.h"
BlynkWeatherOutputDevice *blynkWeatherOutputDevice;
#endif

void setupBlynk() {
#ifdef SUPPORT_BLYNK
    blynkWeatherOutputDevice = new BlynkWeatherOutputDevice(bme280Device, mq135Device, SECRET_BLYNK_TOKEN);
    outputDevices->add(blynkWeatherOutputDevice);
#endif
}

void setupMQTT() {
#ifdef SUPPORT_MQTT
    mqttDevice = new MQTTDevice(
            SECRET_MQTT_SERVER,
            1883,
            SECRET_MQTT_USERNAME,
            SECRET_MQTT_PASSWORD);

    mqttWeatherDevice = new MQTTWeatherOutputDevice(bme280Device, mq135Device, mqttDevice);
    mqttWeatherDevice->configureBME280(MQTT_BME280_TOPIC, MQTT_KEY);
    mqttWeatherDevice->configureBH1750(MQTT_BH1750_TOPIC, MQTT_KEY);
    mqttWeatherDevice->configureMQ135(MQTT_MQ135_TOPIC, MQTT_KEY);
    outputDevices->add(mqttWeatherDevice);
#endif
}

void setupOled() {
#ifdef SUPPORT_OLED
    oledWeatherDevice = new OledWeatherOutputDevice(bme280Device, mq135Device, OLED_I2C_ADDRESS);
    outputDevices->add(oledWeatherDevice);
#endif
}

void setupEpaper() {
#ifdef SUPPORT_EPAPER
    epaperWeatherDevice = new EPaperWeatherOutputDevice(bme280Device, mq135Device);
    outputDevices->add(epaperWeatherDevice);
#endif
}

void process() {
    wifi->reconnect();
    i2cScanner->printDevices();

    Weather weatherEvent;
    bme280Device->update(weatherEvent);
    mq135Device->update(weatherEvent);
    bh1750Device->update(weatherEvent);

    outputDevices->process(weatherEvent);
}

void __unused setup() {
    Serial.begin(9600);

    i2cScanner = new I2CScanner(I2C_SDA_PIN, I2C_SCL_PIN);

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(400000L);

    wifi = new Wifi(SECRET_WIFI_SSID, SECRET_WIFI_PASS);

    bme280Device = new BME280Device();
    mq135Device = new MQ135Device(MQ135_PIN);
    bh1750Device = new BH1750Device();

    outputDevices = new OutputDevices;

    serialWeatherDevice = new SerialWeatherOutputDevice(bme280Device, mq135Device);
    outputDevices->add(serialWeatherDevice);

    setupOled();
    setupMQTT();
    setupBlynk();
    setupEpaper();
    outputDevices->hello();

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

void __unused loop() {
#ifndef DEEP_SLEEP
    process();
    delay(LOOP_INTERVAL_SECONDS * 1000);
#endif
}