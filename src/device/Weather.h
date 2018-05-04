
#ifndef WEATHER_H
#define WEATHER_H

#include <BME280I2C.h>

struct Weather {
    bool bme280Connected;
    bool bh1750Connected;
    bool mq135Connected;

    uint16_t lightLevel;

    float temperature;
    BME280::TempUnit temperatureUnit = BME280::TempUnit_Celsius;
    float pressure;
    BME280::PresUnit pressureUnit = BME280::PresUnit_hPa;
    float relative_humidity;

    float ro;
    float ratio;
    float co2;
};


#endif //WEATHER_H
