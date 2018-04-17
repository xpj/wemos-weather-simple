#ifndef WEATHER_DEVICE_H
#define WEATHER_DEVICE_H

#include "BME280TG.h"

class WeatherOutputDevice {

public:
    WeatherOutputDevice(BME280TG *bme280) {
        bme280TG = bme280;
    }

    virtual void process(const units_t &event280) {};
    virtual void hello() {};

protected:
    BME280TG *bme280TG;

    float temperature(const units_t &event280) { return bme280TG->getTemperature(event280);};
    float pressure(const units_t &event280) { return bme280TG->getPressure(event280);};
    float humidity(const units_t &event280) { return bme280TG->getHumidity(event280);};
};

#endif //WEATHER_DEVICE_H