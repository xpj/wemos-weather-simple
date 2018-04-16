#include "Oled.h"


Oled::Oled(const DevType* dev, uint8_t i2cAddr) {
    oled.begin(dev, i2cAddr);
    oled.clear();
    oled.setFont(System5x7);
}

void Oled::prepare() {
    oled.clear();
    oled.setFont(System5x7);
}

