# wemos-weather-simple
Simple Weather Station

![sws.jpg](sws.jpg "Simple Weather Station")


## Configuration

- in ``src`` folder copy ``secrets_template.h`` to ``secrets.h`` and fill your own
  - Blynk API Token
  - WiFi SSID and Password
  
- ``pio run``


## Libraries

- Adafruit GFX Library
  - pio id #13
  - (https://platformio.org/lib/show/13/Adafruit%20GFX%20Library)
- Adafruit SSD1306
  - pio id #135
  - (https://platformio.org/lib/show/135/Adafruit%20SSD1306)
- BME280
  - pio id #901
  - (https://platformio.org/lib/show/901/BME280)
- SSD1306Ascii
  - pio id #5169
  - (https://platformio.org/lib/show/5169/SSD1306Ascii)
- Blynk
  - pio id #415
  - (https://platformio.org/lib/show/415/Blynk)
