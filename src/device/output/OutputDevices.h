
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

    void process(Weather weatherEvent) {
        for (devices_it = devices.begin(); devices_it!=devices.end(); devices_it++) {
            (*devices_it)->process(weatherEvent);
        }
    }

private:
    std::list<WeatherOutputDevice*> devices;
    std::list<WeatherOutputDevice*>::iterator devices_it;
};

#endif //OUTPUTDEVICES_H
