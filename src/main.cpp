#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "BlynkSimpleEsp8266.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "BME280TG.h"

#include "secrets.h"

#define I2C_ADDRESS 0x3C
#define SCLPIN  5
#define SDAPIN  4

char blynkAuthToken[] = SECRET_BLYNK_TOKEN;

char ssid[] = SECRET_WIFI_SSID;
char pass[] = SECRET_WIFI_PASS;

SSD1306AsciiWire oled;
BME280TG *bme280TG;
BlynkTimer timer;

void toSerial(units_t event280) {
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

void process() {
    units_t event280;
    bme280TG->get(&event280);
    toSerial(event280);
    toDisplay(event280);
    toBlynk(event280);
}

//------------------------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    Wire.begin(SDAPIN, SCLPIN);
    Wire.setClock(400000L);

    oled.begin(&MicroOLED64x48, I2C_ADDRESS);
    Blynk.begin(blynkAuthToken, ssid, pass);
    bme280TG = new BME280TG();
    timer.setInterval(5000L, process);

    delay(50);

    oled.clear();
    oled.setFont(System5x7);
    oled.println("xpj.ninja");
    oled.println();
    oled.println("Simple");
    oled.println("Weather");
    oled.println("Station");

    delay(5000);
}

void loop() {
    Blynk.run();
    timer.run();
}