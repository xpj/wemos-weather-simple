#ifndef OLED_H
#define OLED_H

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

class Oled : public SSD1306AsciiWire {
public:
    Oled(const DevType* dev, uint8_t i2cAddr);

    void prepare();
private:
    SSD1306AsciiWire oled;
};

#endif //OLED_H
