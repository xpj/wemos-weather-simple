#ifndef SENSORDEVICE_H
#define SENSORDEVICE_H

#include "device/Weather.h"

class SensorDevice {
public:
    virtual bool isConnected() {};
    virtual void update(Weather &weatherEvent) {};
};

#endif //SENSORDEVICE_H
