#ifndef EPAPERWEATHEROUTPUTDEVICE_H
#define EPAPERWEATHEROUTPUTDEVICE_H

#include "WeatherOutputDevice.h"

#include <GxEPD.h>
#include <GxGDEH029A1/GxGDEH029A1.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include <Fonts/FreeMonoBold9pt7b.h>

// mapping suggestion from Waveshare 2.9inch e-Paper to Wemos D1 mini
// BUSY -> D6 !!!, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V
// BUSY->D6, RST->D4, DC->D3, CS->D8(SS), CLK->D5(SCK), DIN->D7(MOSI), GND->GND, 3.3V->3.3V
#define EPAPER_RST_PIN 2

// TODO fix this
GxIO_SPI io(SPI, 15, 0); // CS = D0(16), DC = D4(2), RST disabled
GxGDEH029A1 display(io, 2, 12);

class EPaperWeatherOutputDevice: public WeatherOutputDevice {
public:

    EPaperWeatherOutputDevice(BME280Device *bme280Device_m, MQ135Device *mq135Device_m) :
            WeatherOutputDevice(bme280Device_m, mq135Device_m) {
        pinMode(EPAPER_RST_PIN, OUTPUT);
        digitalWrite(EPAPER_RST_PIN, LOW);
        delay(20);
        digitalWrite(EPAPER_RST_PIN, HIGH);
        delay(20);
        display.init(115200);
    }

    void process(Weather weatherEvent) override  {
        display.setRotation(1);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 0);
        display.println();
        if (weatherEvent.bme280Connected) {
            display.print("Temperature : ");
            display.print(temperature(weatherEvent)); display.println(temperatureUnit(weatherEvent));
            display.print("Humidity    : ");
            display.print(humidity(weatherEvent)); display.println("%");
            display.print("Pressure    :");
            display.print(pressure(weatherEvent)); display.println(pressureUnit(weatherEvent));
        } else {
            display.println("No devices connected :-/");
        }
        display.update();
    }

    void hello() override {
        display.setRotation(1);
        display.fillScreen(GxEPD_WHITE);
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold9pt7b);
        display.setCursor(0, 0);
        display.println();
        display.println("xpj.ninja");
        display.println();
        display.println(" Simple");
        display.println(" Weather");
        display.println(" Station");
        display.update();
    }
};

#endif //EPAPERWEATHEROUTPUTDEVICE_H
