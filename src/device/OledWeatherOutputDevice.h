#include "WeatherOutputDevice.h"

#include <SSD1306init.h>
#include <SSD1306AsciiWire.h>

class OledWeatherOutputDevice: public WeatherOutputDevice {
public:
    OledWeatherOutputDevice(BME280Device *bme280, MQ135Device *mq135, uint8_t i2cAddr) : WeatherOutputDevice(bme280, mq135) {
        oled->begin(&MicroOLED64x48, i2cAddr);
    }

    void processBME280(BME280Device::units_t &event) override {
        oled->clear();
        oled->println("Temperature: ");
        oled->print(temperature(event));
        oled->print(" ");
        oled->println(temperatureUnit(event));
        oled->println("Pressure:    ");
        oled->print(pressure(event));
        oled->print(" ");
        oled->println(pressureUnit(event));
        oled->println("Humidity:    ");
        oled->print(humidity(event));
        oled->println(" %");
    }

    void processMQ135(MQ135Device::mq_t &mq135) override {
        oled->clear();
        oled->println("CO2: ");
        oled->print(co2(mq135));
        oled->println(" ppm");
        oled->println("R0:    ");
        oled->print(rzero(mq135));
        oled->println("");
        oled->println("Ratio:    ");
        oled->print(ratio(mq135));
        oled->println(" %");
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