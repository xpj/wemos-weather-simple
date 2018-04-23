#include "WeatherOutputDevice.h"

#include "BlynkSimpleEsp8266.h"

class BlynkWeatherOutputDevice : public WeatherOutputDevice {
public:
    BlynkWeatherOutputDevice(BME280Device *bme280, MQ135Device *mq135, const char* blynkToken) : WeatherOutputDevice(bme280, mq135) {
        Blynk.config(blynkToken);
    }

    void processBME280(BME280Device::units_t &event280) override {
        Blynk.run();
        Blynk.virtualWrite(V0, temperature(event280));
        Blynk.virtualWrite(V1, humidity(event280));
        Blynk.virtualWrite(V2, pressure(event280));
    };
    void processMQ135(MQ135Device::mq_t &mq135) override {
        Blynk.run();
        Blynk.virtualWrite(V3, co2(mq135));
        Blynk.virtualWrite(V4, ratio(mq135));
        Blynk.virtualWrite(V5, rzero(mq135));
    };

};
