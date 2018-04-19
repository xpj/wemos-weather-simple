#ifndef WEATHER_DEVICE_H
#define WEATHER_DEVICE_H

#include "BME280TG.h"

class WeatherOutputDevice {

public:
    WeatherOutputDevice(BME280TG *bme280) {
        bme280TG = bme280;
    }

    virtual void process(units_t &event280) {};
    virtual void hello() {};

protected:
    BME280TG *bme280TG;

    float temperature(units_t &event280) { return bme280TG->getTemperature(event280);};
    float pressure(units_t &event280) { return bme280TG->getPressure(event280);};
    float humidity(units_t &event280) { return bme280TG->getHumidity(event280);};
};

#endif //WEATHER_DEVICE_H