#ifndef MQ135_DEVICE_H
#define MQ135_DEVICE_H

#include <SSD1306AsciiWire.h>
#include "MQ135.h"

class MQ135Device {
public:

    typedef struct {
        float ro;

        float ratio;

        float co2;
    } mq_t;

    MQ135Device(uint8_t pin) {
        mq135 = new MQ135(pin);
        mq135->calibrate();
        Serial.print("Ro = ");
        Serial.println(mq135->getRo());
    }

    void get(mq_t *event) {
        memset(event, 0, sizeof(mq_t));
        event->ro = mq135->getRo();
        event->ratio = mq135->readRatio();
        event->co2 = mq135->readCO2();
    }

    float getRZero(mq_t &event) {
        if (event.ro) {
            return event.ro;
        }
        return -1;
    }
    float getRatio(mq_t &event) {
        if (event.ratio) {
            return event.ratio;
        }
        return -1;
    }
    float getCo2(mq_t &event) {
        if (event.co2) {
            return event.co2;
        }
        return -1;
    }

private:
    MQ135 *mq135;
};

#endif //MQ135_DEVICE_H
