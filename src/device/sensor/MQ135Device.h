#ifndef MQ135_DEVICE_H
#define MQ135_DEVICE_H

#include "SensorDevice.h"
#include <SSD1306AsciiWire.h>
#include "MQ135.h"

class MQ135Device : public SensorDevice {
public:

    MQ135Device(uint8_t pin) {
        mq135 = new MQ135(pin);
        mq135->calibrate();
    }

    bool isConnected() {
        return false;
    }

    void update(Weather &weatherEvent) {
        weatherEvent.mq135Connected = isConnected();
        weatherEvent.ro = mq135->getRo();
        weatherEvent.ratio = mq135->readRatio();
        weatherEvent.co2 = mq135->readCO2();
    }

    float getRZero(Weather event) {
        if (event.ro) {
            return event.ro;
        }
        return -1;
    }
    float getRatio(Weather event) {
        if (event.ratio) {
            return event.ratio;
        }
        return -1;
    }
    float getCo2(Weather event) {
        if (event.co2) {
            return event.co2;
        }
        return -1;
    }

private:
    MQ135 *mq135;
};

#endif //MQ135_DEVICE_H
