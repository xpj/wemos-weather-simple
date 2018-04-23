#ifndef WEATHER_DEVICE_H
#include "WeatherOutputDevice.h"
#endif

class SerialWeatherOutputDevice: public WeatherOutputDevice {

public:
    SerialWeatherOutputDevice(BME280Device *bme280, MQ135Device *mq135) : WeatherOutputDevice(bme280, mq135) {
    }

    void processBME280(BME280Device::units_t &event) override {
        Serial.print("Temperature: ");
        Serial.print(temperature(event));
        Serial.println(" C");
        Serial.print("Pressure:    ");
        Serial.print(pressure(event));
        Serial.println(" hPa");
        Serial.print("Humidity:    ");
        Serial.print(humidity(event));
        Serial.println(" %");
        Serial.println("--------------");
    }

    void processMQ135(MQ135Device::mq_t &mq135) override {
        Serial.print("CO2: ");
        Serial.print(co2(mq135));
        Serial.println(" ppm");
        Serial.print("R0:    ");
        Serial.print(rzero(mq135));
        Serial.println("");
        Serial.print("Ratio:    ");
        Serial.print(ratio(mq135));
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