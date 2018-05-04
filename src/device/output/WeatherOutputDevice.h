#ifndef WEATHER_DEVICE_H
#define WEATHER_DEVICE_H

#include "device/Weather.h"

#include "device/sensor/BME280Device.h"
#include "device/sensor/MQ135Device.h"

class WeatherOutputDevice {

public:
    WeatherOutputDevice(BME280Device *bme280Device_m, MQ135Device *mq135Device_m) {
        bme280Device = bme280Device_m;
        mq135Device = mq135Device_m;
    }

    virtual void process(Weather weatherEvent) {};

    virtual void hello() {};

protected:
    BME280Device *bme280Device;
    MQ135Device *mq135Device;

    float temperature(Weather event) { return bme280Device->getTemperature(event);};
    float pressure(Weather event) { return bme280Device->getPressure(event);};
    float humidity(Weather event) { return bme280Device->getHumidity(event);};

    const char* temperatureUnit(Weather event) {  return bme280Device->getTemperatureUnitName(event.temperatureUnit); }
    const char* pressureUnit(Weather event) {  return bme280Device->getPressureUnitName(event.pressureUnit); }

    float rzero(Weather event) { return mq135Device->getRZero(event);};
    float ratio(Weather event) { return mq135Device->getRatio(event);};
    float co2(Weather event) { return mq135Device->getCo2(event);};


};

#endif //WEATHER_DEVICE_H