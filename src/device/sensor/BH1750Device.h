#ifndef BH1750DEVICE_H
#define BH1750DEVICE_H

#include <BH1750.h>
#include "SensorDevice.h"
#include "../Weather.h"

class BH1750Device: public SensorDevice {
public:
    BH1750Device(byte addr = 0x23) {
        lightMeter = new BH1750(addr);
    }

    bool isConnected() override {
        return lightMeter->begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
    }

    void update(Weather &weatherEvent) override {
        weatherEvent.bh1750Connected = isConnected();
        if (isConnected()) {
            uint16_t lux = lightMeter->readLightLevel();
            weatherEvent.lightLevel = lux;
        }
    }

private:
    BH1750 *lightMeter;
};

#endif //BH1750DEVICE_H
