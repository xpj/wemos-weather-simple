#include "WeatherOutputDevice.h"

#include "BlynkSimpleEsp8266.h"

class BlynkWeatherOutputDevice : public WeatherOutputDevice {
public:
    BlynkWeatherOutputDevice(BME280TG *bme280, const char* blynkToken) : WeatherOutputDevice(bme280) {
        Blynk.config(blynkToken);
    }

    void process(units_t &event280) override {
        Blynk.run();
        Blynk.virtualWrite(V0, temperature(event280));
        Blynk.virtualWrite(V1, humidity(event280));
        Blynk.virtualWrite(V2, pressure(event280));
    }
};
