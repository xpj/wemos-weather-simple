#include "WeatherOutputDevice.h"

#include <SSD1306init.h>
#include <SSD1306AsciiWire.h>

class OledWeatherOutputDevice: public WeatherOutputDevice {
public:
    OledWeatherOutputDevice(BME280Device *bme280, MQ135Device *mq135, uint8_t i2cAddr) : WeatherOutputDevice(bme280, mq135) {
        oled->begin(&MicroOLED64x48, i2cAddr);
    }

    void process(Weather weatherEvent) override {
        oled->clear();
        if (weatherEvent.bme280Connected) {
            oled->println("Temperature: ");
            oled->print(temperature(weatherEvent));
            oled->print(" ");
            oled->println(temperatureUnit(weatherEvent));
            oled->println("Pressure:    ");
            oled->print(pressure(weatherEvent));
            oled->print(" ");
            oled->println(pressureUnit(weatherEvent));
            oled->println("Humidity:    ");
            oled->print(humidity(weatherEvent));
            oled->println(" %");
        }
        if (weatherEvent.mq135Connected) {
            oled->clear();
            oled->println("CO2: ");
            oled->print(co2(weatherEvent));
            oled->println(" ppm");
            oled->println("R0:    ");
            oled->print(rzero(weatherEvent));
            oled->println("");
            oled->println("Ratio:    ");
            oled->print(ratio(weatherEvent));
            oled->println(" %");
        }
    }

    void hello() override {
        oled->clear();
        oled->setFont(font);
        oled->println("xpj.ninja");
        oled->println();
        oled->println("Simple");
        oled->println("Weather");
        oled->println("Station");
    }

private:
    SSD1306AsciiWire *oled = new SSD1306AsciiWire();
    const unsigned char *font = System5x7;
};