#ifndef WEATHER_DEVICE_H
#define WEATHER_DEVICE_H

#include "BME280Device.h"
#include "MQ135Device.h"

class WeatherOutputDevice {

public:
    WeatherOutputDevice(BME280Device *bme280Device_m, MQ135Device *mq135Device_m) {
        bme280Device = bme280Device_m;
        mq135Device = mq135Device_m;
    }

    virtual void processBME280(BME280Device::units_t &event280) {};
    virtual void processMQ135(MQ135Device::mq_t &mq135) {};

    virtual void hello() {};

protected:
    BME280Device *bme280Device;
    MQ135Device *mq135Device;

    float temperature(BME280Device::units_t &event280) { return bme280Device->getTemperature(event280);};
    float pressure(BME280Device::units_t &event280) { return bme280Device->getPressure(event280);};
    float humidity(BME280Device::units_t &event280) { return bme280Device->getHumidity(event280);};

    float rzero(MQ135Device::mq_t &mq135) { return mq135Device->getRZero(mq135);};
    float ratio(MQ135Device::mq_t &mq135) { return mq135Device->getRatio(mq135);};
    float co2(MQ135Device::mq_t &mq135) { return mq135Device->getCo2(mq135);};


};

#endif //WEATHER_DEVICE_H