#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "BME280TG.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
#define SCLPIN  5
#define SDAPIN  4

SSD1306AsciiWire oled;
BME280TG *bme280TG;

//------------------------------------------------------------------------------
void setup() {
    Serial.begin(9600);
    Wire.begin(SDAPIN, SCLPIN);
    Wire.setClock(400000L);
    delay(50);
    oled.begin(&MicroOLED64x48, I2C_ADDRESS);
    oled.clear();
    oled.setFont(System5x7);
    oled.println("xpj.ninja");
    oled.println();
    oled.println("Simple");
    oled.println("Weather");
    oled.println("Station");

    bme280TG = new BME280TG();
    delay(5000);
}

void process() {
    units_t event280;
    bme280TG->get(&event280);
    Serial.print("Press280:    ");
    Serial.print(bme280TG->getPressure(event280));
    Serial.println(" hPa");
    Serial.print("Temp 280:    ");
    Serial.print(bme280TG->getTemperature(event280));
    Serial.println(" C");
    Serial.print("Hum 280:     ");
    Serial.print(bme280TG->getHumidity(event280));
    Serial.println(" %");

    oled.clear();
    oled.setFont(System5x7);
    oled.println("Temperature");
    oled.println(bme280TG->getTemperature(event280));
    oled.println("Humidity");
    oled.print(bme280TG->getHumidity(event280)); oled.println("%");
    oled.println("Pressure");
    oled.print(bme280TG->getPressure(event280)); oled.println(" hPa");
}
void loop() {
    process();
    delay(5000);
}