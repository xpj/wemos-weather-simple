#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "AdafruitIO_WiFi.h"
#include "BlynkSimpleEsp8266.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "BME280TG.h"
#include "ArduinoJson.h"

#include "secrets.h"

#define I2C_ADDRESS 0x3C
#define SCLPIN  5
#define SDAPIN  4

#define AIO_GROUP "simple-weather-station"
#define AIO_TEMP "temperature"
#define AIO_HUMIDITY "humidity"
#define AIO_PRESSURE "pressure"

ESP8266WebServer webServer(80);
SSD1306AsciiWire oled;
BME280TG *bme280TG;
BlynkTimer timer;
AdafruitIO_WiFi aio(SECRET_AIO_USERNAME, SECRET_AIO_TOKEN, SECRET_WIFI_SSID, SECRET_WIFI_PASS);
AdafruitIO_Group *group = aio.group(AIO_GROUP);


void toSerial(units_t event280) {
    Serial.println("---------------");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("---------------");
    Serial.print("Temperature: ");
    Serial.print(bme280TG->getTemperature(event280));
    Serial.println(" C");
    Serial.print("Pressure:    ");
    Serial.print(bme280TG->getPressure(event280));
    Serial.println(" hPa");
    Serial.print("Humidity:    ");
    Serial.print(bme280TG->getHumidity(event280));
    Serial.println(" %");
}

void toDisplay(units_t event280) {
    oled.clear();
    oled.setFont(System5x7);
    oled.println("Temperature");
    oled.println(bme280TG->getTemperature(event280));
    oled.println("Humidity");
    oled.print(bme280TG->getHumidity(event280)); oled.println("%");
    oled.println("Pressure");
    oled.print(bme280TG->getPressure(event280)); oled.println(" hPa");

}

void toBlynk(units_t event280) {
    Blynk.virtualWrite(V0, bme280TG->getTemperature(event280));
    Blynk.virtualWrite(V1, bme280TG->getHumidity(event280));
    Blynk.virtualWrite(V2, bme280TG->getPressure(event280));
}

void toAio(units_t event280) {
    group->set(AIO_TEMP, bme280TG->getTemperature(event280));
    group->set(AIO_HUMIDITY, bme280TG->getHumidity(event280));
    group->set(AIO_PRESSURE, bme280TG->getPressure(event280));
    group->save();
}

void process() {
    units_t event280;
    bme280TG->get(&event280);
    toSerial(event280);
    toDisplay(event280);
    toBlynk(event280);
    toAio(event280);
}

String pingStatus() {
    String output;
    StaticJsonBuffer<200> jsonBuffer;

    JsonObject& pong = jsonBuffer.createObject();

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

void endpointPing() {
    webServer.on("/ping",[] () {
        webServer.send(200, "text/json", pingStatus());
    });
}

void setup() {
    Serial.begin(9600);
    Wire.begin(SDAPIN, SCLPIN);
    Wire.setClock(400000L);

    oled.begin(&MicroOLED64x48, I2C_ADDRESS);
    oled.clear();
    oled.setFont(System5x7);
    oled.println("xpj.ninja");
    oled.println();
    oled.println("Simple");
    oled.println("Weather");
    oled.println("Station");

    bme280TG = new BME280TG();

    Blynk.begin(SECRET_BLYNK_TOKEN, SECRET_WIFI_SSID, SECRET_WIFI_PASS);

    aio.connect();
    while(aio.status() < AIO_CONNECTED) {
        delay(500);
    }
    timer.setInterval(5000L, process);
    endpointPing();
    webServer.begin();

    delay(5000);
}

void loop() {
    aio.run();
    Blynk.run();
    timer.run();
    webServer.handleClient();
}