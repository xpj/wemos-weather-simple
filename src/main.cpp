#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#include <PubSubClient.h>
#include <utility>
#include <SSD1306init.h>
#include "BME280TG.h"
#include "ArduinoJson.h"

#include "Wifi.h"
#include "SerialWeatherOutputDevice.h"
#include "OledWeatherOutputDevice.h"
#include "MQTTWeatherOutputDevice.h"

#include "config.h"

#define LOOP_INTERVAL 5000

#define I2C_ADDRESS 0x3C
#define SCLPIN  5
#define SDAPIN  4

BME280TG *bme280TG;

SerialWeatherOutputDevice *serialWeatherDevice;
OledWeatherOutputDevice *oledWeatherDevice;
Wifi *wifi;
MQTTWeatherOutputDevice *mqttWeatherOutputDevice;

#ifdef SUPPORT_BLYNK

#include "BlynkSimpleEsp8266.h"

BlynkTimer timer;

void toBlynk(units_t event280) {
    Blynk.virtualWrite(V0, bme280TG->getTemperature(event280));
    Blynk.virtualWrite(V1, bme280TG->getHumidity(event280));
    Blynk.virtualWrite(V2, bme280TG->getPressure(event280));
}
#endif

#ifdef SUPPORT_ADAFRUIT_IO

#include "AdafruitIO_WiFi.h"
#define AIO_GROUP "simple-weather-station"
#define AIO_TEMP "temperature"
#define AIO_HUMIDITY "humidity"
#define AIO_PRESSURE "pressure"


AdafruitIO_WiFi aio(SECRET_AIO_USERNAME, SECRET_AIO_TOKEN, SECRET_WIFI_SSID, SECRET_WIFI_PASS);
AdafruitIO_Group *group = aio.group(AIO_GROUP);

void toAio(units_t event280) {
    group->set(AIO_TEMP, bme280TG->getTemperature(event280));
    group->set(AIO_HUMIDITY, bme280TG->getHumidity(event280));
    group->set(AIO_PRESSURE, bme280TG->getPressure(event280));
    group->save();
}

void setupAio() {
    aio.connect();
    while(aio.status() < AIO_CONNECTED) {
        delay(500);
    }
}
#endif

void process() {
    units_t event280;
    bme280TG->get(&event280);

    serialWeatherDevice->process(event280);
    oledWeatherDevice->process(event280);
    mqttWeatherOutputDevice->process(event280);

    #ifdef SUPPORT_BLYNK
    toBlynk(event280);
#endif
#ifdef SUPPORT_ADAFRUIT_IO
    toAio(event280);
#endif
}

void setup() {
    Wire.begin(SDAPIN, SCLPIN);
    Wire.setClock(400000L);

    bme280TG = new BME280TG();
    serialWeatherDevice = new SerialWeatherOutputDevice(bme280TG);
    serialWeatherDevice->hello();
    oledWeatherDevice = new OledWeatherOutputDevice(bme280TG, I2C_ADDRESS);
    oledWeatherDevice->hello();

    wifi = new Wifi(SECRET_WIFI_SSID, SECRET_WIFI_PASS);
    mqttWeatherOutputDevice = new MQTTWeatherOutputDevice(
            bme280TG,
            SECRET_MQTT_SERVER,
            1883,
            SECRET_MQTT_USERNAME,
            SECRET_MQTT_PASSWORD,
            "sensors/sws1/bme280",
            "sws",
            wifi->getWiFiClient());

#ifdef SUPPORT_BLYNK
    Blynk.config(SECRET_BLYNK_TOKEN);
    timer.setInterval(LOOP_INTERVAL, process);
#endif

#ifdef SUPPORT_ADAFRUIT_IO
    setupAio();
#endif

    delay(5000);
}

void loop() {
#ifdef SUPPORT_ADAFRUIT_IO
    aio.run();
#endif
#ifdef SUPPORT_BLYNK
    timer.run();
    Blynk.run();
#else
    process();
    delay(LOOP_INTERVAL);
#endif
}