#ifndef WEATHER_DEVICE_H
#include "WeatherOutputDevice.h"
#endif

class SerialWeatherOutputDevice: public WeatherOutputDevice {

public:
    SerialWeatherOutputDevice(BME280TG *bme280) : WeatherOutputDevice(bme280TG) {
    }

    void process(units_t &event280) override {
        Serial.print("Temperature: ");
        Serial.print(temperature(event280));
        Serial.println(" C");
        Serial.print("Pressure:    ");
        Serial.print(pressure(event280));
        Serial.println(" hPa");
        Serial.print("Humidity:    ");
        Serial.print(humidity(event280));
        Serial.println(" %");
        Serial.println("--------------");
    }

    void hello() override {
        Serial.println("xpj.ninja");
        Serial.println();
        Serial.println(" Simple");
        Serial.println(" Weather");
        Serial.println(" Station");
        Serial.println("---------");
    }
};