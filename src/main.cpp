#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include <utility>
#include <SSD1306init.h>

#include "config.h"

#include "ArduinoJson.h"
#include "Wifi.h"

#include "SerialWeatherOutputDevice.h"
#include "MQ135Device.h"
#include "OutputDevices.h"

BME280Device *bme280Device;
MQ135Device *mq135Device;

Wifi *wifi;

OutputDevices *outputDevices;

SerialWeatherOutputDevice *serialWeatherDevice;
#ifdef SUPPORT_OLED
#include "OledWeatherOutputDevice.h"
OledWeatherOutputDevice *oledWeatherDevice;
#endif

#ifdef SUPPORT_MQTT
#include "MQTTDevice.h"
#include "MQTTWeatherOutputDevice.h"
MQTTDevice *mqttDevice;
MQTTWeatherOutputDevice *mqttWeatherDevice;
#endif

#ifdef SUPPORT_BLYNK
#include "BlynkWeatherOutputDevice.h"

BlynkWeatherOutputDevice *blynkWeatherOutputDevice;
#endif

void process() {
    wifi->reconnect();

    BME280Device::units_t eventBme280;
    bme280Device->get(&eventBme280);

    MQ135Device::mq_t eventMq135;
    mq135Device->get(&eventMq135);

    outputDevices->processMQ135(eventMq135);
    outputDevices->processBME280(eventBme280);
}

void setup() {
    Serial.begin(9600);
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(400000L);

    wifi = new Wifi(SECRET_WIFI_SSID, SECRET_WIFI_PASS);

    bme280Device = new BME280Device();
    mq135Device = new MQ135Device(MQ135_PIN);

    outputDevices = new OutputDevices;

    serialWeatherDevice = new SerialWeatherOutputDevice(bme280Device, mq135Device);
    outputDevices->add(serialWeatherDevice);
#ifdef SUPPORT_OLED
    oledWeatherDevice = new OledWeatherOutputDevice(bme280Device, mq135Device, OLED_I2C_ADDRESS);
    outputDevices->add(oledWeatherDevice);
#endif

#ifdef SUPPORT_MQTT
    mqttDevice = new MQTTDevice(
            SECRET_MQTT_SERVER,
            1883,
            SECRET_MQTT_USERNAME,
            SECRET_MQTT_PASSWORD);

    mqttWeatherDevice = new MQTTWeatherOutputDevice(bme280Device, mq135Device, mqttDevice);
    mqttWeatherDevice->configureBME280(MQTT_BME280_TOPIC, MQTT_KEY);
    mqttWeatherDevice->configureMQ135(MQTT_MQ135_TOPIC, MQTT_KEY);
    outputDevices->add(mqttWeatherDevice);
#endif

#ifdef SUPPORT_BLYNK
    blynkWeatherOutputDevice = new BlynkWeatherOutputDevice(bme280Device, mq135Device, SECRET_BLYNK_TOKEN);
    outputDevices->add(blynkWeatherOutputDevice);
#endif

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

void loop() {
#ifndef DEEP_SLEEP
    process();
    delay(LOOP_INTERVAL_SECONDS * 1000);
#endif
}