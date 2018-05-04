#ifndef BME280DEVICE_H
#define BME280DEVICE_H

#include "SensorDevice.h"
#include <BME280I2C.h>

class BME280Device: public SensorDevice {
public:

    BME280Device(BME280::TempUnit temperatureUnit_m = BME280::TempUnit_Celsius,
                 BME280::PresUnit presUnit_m = BME280::PresUnit_hPa) {
        tempUnit = temperatureUnit_m;
        presUnit = presUnit_m;
        if (!isConnected()) {
            Serial.println("Could not find a valid BMP280 sensor, check wiring!");
        }
    }

    bool isConnected() {
        return bme280.begin();
    }

    void update(Weather &weatherEvent) {
        weatherEvent.bme280Connected = isConnected();
        if (isConnected()) {
            float pressure(NAN), temperature(NAN), relative_humidity(NAN);
            bme280.read(pressure, temperature, relative_humidity, tempUnit, presUnit);

            weatherEvent.pressure = pressure;
            weatherEvent.pressureUnit = presUnit;
            weatherEvent.temperature = temperature;
            weatherEvent.temperatureUnit = tempUnit;
            weatherEvent.relative_humidity = relative_humidity;

            weatherEvent.temperatureUnit = tempUnit;
            weatherEvent.pressureUnit = presUnit;
        }
    }

    float getTemperature(Weather event) {
        if (event.temperature) {
            return event.temperature;
        }
    }

    float getPressure(Weather event) {
        if (event.pressure) {
            return event.pressure;
        }
    }

    float getHumidity(Weather event) {
        if (event.relative_humidity) {
            return event.relative_humidity;
        }
    }

    const char* getTemperatureUnitName(BME280::TempUnit tempUnit) { return tempUnitStrings[tempUnit]; }
    const char* getPressureUnitName(BME280::PresUnit presUnit) { return presUnitStrings[presUnit]; }

private:
    BME280I2C bme280;
    BME280::TempUnit tempUnit;
    BME280::PresUnit presUnit;

    const char *tempUnitStrings[2] = { "C", "F" };
    const char *presUnitStrings[7] = { "Pa", "hPa", "inHg", "atm", "bar", "torr", "psi" };
};


#endif //BME280DEVICE_H
