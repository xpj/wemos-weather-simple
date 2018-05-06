#ifndef WEATHER_DEVICE_H
#include "WeatherOutputDevice.h"
#endif

class SerialWeatherOutputDevice: public WeatherOutputDevice {

public:
    SerialWeatherOutputDevice(BME280Device *bme280, MQ135Device *mq135) : WeatherOutputDevice(bme280, mq135) {
    }

    void process(Weather weatherEvent) override {
        if (weatherEvent.bme280Connected) {
            Serial.print("Temperature: ");
            Serial.print(temperature(weatherEvent));
            Serial.print(" ");
            Serial.println(temperatureUnit(weatherEvent));
            Serial.print("Pressure:    ");
            Serial.print(pressure(weatherEvent));
            Serial.print(" ");
            Serial.println(pressureUnit(weatherEvent));
            Serial.print("Humidity:    ");
            Serial.print(humidity(weatherEvent));
            Serial.println(" %");
            Serial.println("--------------");
        }
        if (weatherEvent.bh1750Connected) {
            Serial.print("Light level: ");
            Serial.print(lightLevel(weatherEvent));
            Serial.println(" lux");
            Serial.println("--------------");
        }
        if (weatherEvent.mq135Connected) {
            Serial.print("CO2: ");
            Serial.print(co2(weatherEvent));
            Serial.println(" ppm");
            Serial.print("R0:    ");
            Serial.print(rzero(weatherEvent));
            Serial.println("");
            Serial.print("Ratio:    ");
            Serial.print(ratio(weatherEvent));
            Serial.println(" %");
            Serial.println("--------------");
        }
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