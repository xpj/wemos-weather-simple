#include "WeatherOutputDevice.h"

#include "AdafruitIO_WiFi.h"
#define AIO_GROUP "simple-weather-station"
#define AIO_TEMP "temperature"
#define AIO_HUMIDITY "humidity"
#define AIO_PRESSURE "pressure"

class AdafruitIOWeatherOutputDevice:  public WeatherOutputDevice {
public:
    AdafruitIOWeatherOutputDevice(BME280TG *bme280, const char* aioGroup, const char* aioUsername, const char* aioToken, const char* wifiSsid, const char* wifiPass) : WeatherOutputDevice(bme280) {
         aio = new AdafruitIO_WiFi(aioUsername, aioToken, wifiPass, wifiSsid);
         group = aio->group(aioGroup);
    }

    void process(units_t &event280) override {
        if (isAIOConnected()) {
            aio->run();

            group->set(AIO_TEMP, temperature(event280));
            group->set(AIO_HUMIDITY, humidity(event280));
            group->set(AIO_PRESSURE, pressure(event280));
            group->save();
        }
    }

    bool isAIOConnected() {
        if (aio->status() < AIO_CONNECTED) {
            Serial.println("AdafruitIO Not Connected...");
            long now = millis();
            if (now - lastReconnectAttempt > 5000) {
                lastReconnectAttempt = now;
                aio->connect();
                delay(500);
                if (aio->status() < AIO_CONNECTED) {
                    lastReconnectAttempt = 0;
                    Serial.println("MQTT connected");
                }
            }
        }
        return (aio->status() < AIO_CONNECTED);
    }

private:
    AdafruitIO_WiFi *aio;
    AdafruitIO_Group *group;
    long lastReconnectAttempt = 0;
};