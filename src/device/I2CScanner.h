#ifndef I2CSCANNER_H
#define I2CSCANNER_H

#include <Wire.h>
#include "Arduino.h"

class I2CScanner {
public:
    I2CScanner(uint8_t sdaPin_m, uint8_t sclPin_m) {
        sdaPin = sdaPin_m;
        sclPin = sclPin_m;
        printDevices();
    }

    void printDevices() {
        printDevices(sdaPin, sclPin);
    }

    void printDevices(uint8_t sdaPin, uint8_t sclPin) {
        byte error, address;
        int nDevices;

        Wire.begin(sdaPin, sclPin);
        Serial.println();
        Serial.print("I2C Devices: ");

        nDevices = 0;
        for (address = 1; address < 127; address++) {
            // The i2c scanner uses the return value of
            // the Write.endTransmisstion to see if
            // a device did acknowledge to the address.
            Wire.beginTransmission(address);
            error = Wire.endTransmission();

            if (error == 0) {
                Serial.print("[0x");
                if (address < 16) {
                    Serial.print("0");
                }
                Serial.print(address, HEX);
                Serial.print("|OK]");

                nDevices++;
            } else if (error == 4) {
                Serial.print("[0x");
                if (address < 16) {
                    Serial.print("0");
                }
                Serial.print(address, HEX);
                Serial.print("|ER]");
            }
        }
        if (nDevices == 0) {
            Serial.println("No I2C devices found\n");
        } else {
            Serial.println();
        }
    }

private:
    uint8_t sdaPin;
    uint8_t sclPin;

};


#endif //I2CSCANNER_H
