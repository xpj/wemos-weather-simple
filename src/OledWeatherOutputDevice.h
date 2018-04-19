#include "WeatherOutputDevice.h"

#include <SSD1306init.h>
#include <SSD1306AsciiWire.h>

class OledWeatherOutputDevice: public WeatherOutputDevice {
public:
    OledWeatherOutputDevice(BME280TG *bme280, uint8_t i2cAddr) : WeatherOutputDevice(bme280) {
        oled->begin(&MicroOLED64x48, i2cAddr);
    }

    void process(units_t &event280) override {
        oled->clear();
        oled->setFont(font);
        oled->println("Temperature");
        oled->println(temperature(event280));
        oled->println("Humidity");
        oled->print(humidity(event280));
        oled->println("%");
        oled->println("Pressure");
        oled->print(pressure(event280));
        oled->println(" hPa");
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