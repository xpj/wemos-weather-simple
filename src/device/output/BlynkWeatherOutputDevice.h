#include "WeatherOutputDevice.h"

#include "BlynkSimpleEsp8266.h"

class BlynkWeatherOutputDevice : public WeatherOutputDevice {
public:
    BlynkWeatherOutputDevice(BME280Device *bme280, MQ135Device *mq135, const char* blynkToken) : WeatherOutputDevice(bme280, mq135) {
        Blynk.config(blynkToken);
    }

    void process(Weather weatherEvent) override {
        Blynk.run();
        if (weatherEvent.bme280Connected) {
            Blynk.virtualWrite(V0, temperature(weatherEvent));
            Blynk.virtualWrite(V1, humidity(weatherEvent));
            Blynk.virtualWrite(V2, pressure(weatherEvent));
        }
        if (weatherEvent.mq135Connected) {
            Blynk.virtualWrite(V3, co2(weatherEvent));
            Blynk.virtualWrite(V4, ratio(weatherEvent));
            Blynk.virtualWrite(V5, rzero(weatherEvent));
        }
    };

};
