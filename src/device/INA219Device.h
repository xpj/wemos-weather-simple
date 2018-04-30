
#ifndef INA219DEVICE_H
#define INA219DEVICE_H

#include <Wire.h>
#include <Adafruit_INA219.h>

class INA219Device {
public:

    typedef struct {
        float shuntvoltage = 0;
        float busvoltage = 0;
        float current_mA = 0;
        float loadvoltage = 0;
        float power_mW = 0;
    } ina219_t;

    INA219Device() {
        ina219.begin();
    }

    void get(ina219_t *event) {
        memset(event, 0, sizeof(ina219_t));
        event->busvoltage = ina219.getBusVoltage_V();
        event->current_mA = ina219.getBusVoltage_V();
        event->shuntvoltage = ina219.getShuntVoltage_mV();
        event->power_mW = ina219.getPower_mW();
        event->loadvoltage = event->busvoltage + (event->shuntvoltage / 1000);
    }

    void print(ina219_t *event) {
        Serial.print("Bus Voltage:   "); Serial.print(event->busvoltage); Serial.println(" V");
        Serial.print("Shunt Voltage: "); Serial.print(event->shuntvoltage); Serial.println(" mV");
        Serial.print("Load Voltage:  "); Serial.print(event->loadvoltage); Serial.println(" V");
        Serial.print("Current:       "); Serial.print(event->current_mA); Serial.println(" mA");
        Serial.print("Power:         "); Serial.print(event->power_mW); Serial.println(" mW");
        Serial.println("");
    }

private:
    Adafruit_INA219 ina219;
};

#endif //INA219DEVICE_H
