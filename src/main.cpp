#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WebServer.h>
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

ESP8266WebServer webServer(80);

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

String pingStatus() {
    String output;
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject &pong = jsonBuffer.createObject();

    pong["chipId"] = ESP.getChipId();
    pong["sdkVersion"] = ESP.getSdkVersion();
    pong["coreVersion"] = ESP.getCoreVersion();
    pong["bootVersion"] = ESP.getBootVersion();
    pong["cpuFreqMHz"] = ESP.getCpuFreqMHz();
    pong["freeHeap"] = ESP.getFreeHeap();

    pong.printTo(output);

    return output;
}

//------------------------------------------------------------------------------

String wotJson() {
    String output;
    StaticJsonBuffer<200> jsonBuffer;

    JsonArray &json = jsonBuffer.createArray();
    JsonObject &root = json.createNestedObject();

    root["name"] = "xpj.ninja/simple-weather-station";
    root["type"] = "thing";
    root["description"] = "xpj.ninja/simple-weather-station";

    // properties
    JsonObject &properties = root.createNestedObject("properties");

    JsonObject &temperatureProperty = jsonBuffer.createObject();
    temperatureProperty["type"] = "number";
    temperatureProperty["unit"] = "celsius";
    temperatureProperty["description"] = "Temperature Sensor";
    temperatureProperty["href"] = "/properties/temperature";

    properties["temperature"] = temperatureProperty;

    // events


    json.printTo(output);
    return output;
}

void endpointWot() {
    webServer.on("/things/esp", []() {
        webServer.send(200, "text/json", wotJson());
    });
}

void endpointTemperature() {
    webServer.on("/things/esp/properties/temperature", []() {
        units_t event280;
        bme280TG->get(&event280);
        char buff[10];
        dtostrf(bme280TG->getTemperature(event280), 4, 6, buff);
        String temp = "{\"temperature\":";
        temp += buff;
        temp += "}";
        webServer.send(200, "text/json", temp);
    });
}

void endpointPing() {
    webServer.on("/ping", []() {
        webServer.send(200, "text/json", pingStatus());
    });
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

    endpointPing();
    endpointTemperature();
    endpointWot();

    webServer.begin();

    delay(5000);
}

void loop() {
    webServer.handleClient();
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