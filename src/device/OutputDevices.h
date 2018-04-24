
#ifndef OUTPUTDEVICES_H
#define OUTPUTDEVICES_H

#include "WeatherOutputDevice.h"
#include <stdlib.h>
#include <list>

class OutputDevices {
public:
    void add(WeatherOutputDevice *dev) {
        devices.push_back(dev);
    }

    void hello() {
        for (devices_it = devices.begin(); devices_it!=devices.end(); devices_it++) {
            (*devices_it)->hello();
        }
    }
    void processBME280(BME280Device::units_t &event) {
        for (devices_it = devices.begin(); devices_it!=devices.end(); devices_it++) {
            (*devices_it)->processBME280(event);
        }
    }
    void processMQ135(MQ135Device::mq_t &event) {
        for (devices_it = devices.begin(); devices_it!=devices.end(); devices_it++) {
            (*devices_it)->processMQ135(event);
        }
    }

private:
    std::list<WeatherOutputDevice*> devices;
    std::list<WeatherOutputDevice*>::iterator devices_it;
};

#endif //OUTPUTDEVICES_H
