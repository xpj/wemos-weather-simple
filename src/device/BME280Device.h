#ifndef BME280DEVICE_H
#define BME280DEVICE_H

#include <BME280I2C.h>

class BME280Device {
public:

    typedef struct {
        float temperature;
        BME280::TempUnit temperatureUnit;

        float pressure;
        BME280::PresUnit pressureUnit;

        float relative_humidity;
    } units_t;

    BME280Device(BME280::TempUnit temperatureUnit_m = BME280::TempUnit_Celsius,
                 BME280::PresUnit presUnit_m = BME280::PresUnit_hPa) {
        tempUnit = temperatureUnit_m;
        presUnit = presUnit_m;
        if (!bme280.begin()) {
            Serial.println("Could not find a valid BMP280 sensor, check wiring!");
            while (1);
        }
    }

    void get(units_t *event) {
        float pressure(NAN), temperature(NAN), relative_humidity(NAN);
        bme280.read(pressure, temperature, relative_humidity, tempUnit, presUnit);

        memset(event, 0, sizeof(units_t));
        event->pressure = pressure;
        event->pressureUnit = presUnit;
        event->temperature = temperature;
        event->temperatureUnit = tempUnit;
        event->relative_humidity = relative_humidity;
    }

    float getTemperature(units_t event) {
        if (event.temperature) {
            return event.temperature;
        }
    }

    float getPressure(units_t event) {
        if (event.pressure) {
            return event.pressure;
        }
    }

    float getHumidity(units_t event) {
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
