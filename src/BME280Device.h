#ifndef BME280DEVICE_H
#define BME280DEVICE_H

#include <BME280I2C.h>

class BME280Device {
public:
    typedef enum {
        TEMP_C = (1),
        TEMP_F = (2),

    } temp_unit;

    typedef enum {
        // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
                PA = (0),
        HPA = (1),
        HG = (2),
        ATM = (3),
        BAR = (4),
        TORR = (5),
        NM2 = (6),
        PSI = (7)
    } pressure_unit;

    typedef struct {
        float temperature;
        temp_unit temperatureUnit;

        float pressure;
        pressure_unit pressureUnit;

        float relative_humidity;
    } units_t;

    BME280Device() {
        if (!bme280.begin()) {
            Serial.println("Could not find a valid BMP280 sensor, check wiring!");
            while (1);
        }
    }

    void get(units_t *event) {
        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_hPa);

        float pressure(NAN), temperature(NAN), relative_humidity(NAN);
        bme280.read(pressure, temperature, relative_humidity, tempUnit, presUnit);

        memset(event, 0, sizeof(units_t));
        event->pressure = pressure;
        event->pressureUnit = HPA;
        event->temperature = temperature;
        event->temperatureUnit = TEMP_C;
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

private:
    BME280I2C bme280;
};


#endif //BME280DEVICE_H
