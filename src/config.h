
#ifndef WEMOSWEATHERSIMPLE_CONFIG_H
#define WEMOSWEATHERSIMPLE_CONFIG_H

#include "secrets.h"

#define SWS1
//#undef SWS1
#ifdef SWS1
#include "config-sws1.h"
#endif

//#define SWS2
#undef SWS2
#ifdef SWS2
#include "config-sws2.h"
#endif

#define MQ135_PIN  A0
#define OLED_I2C_ADDRESS 0x3C
#define I2C_SCL_PIN  5
#define I2C_SDA_PIN  4

#define LOOP_INTERVAL_SECONDS 10
#define DEEP_SLEEP_INTERVAL_SECONDS 60

#endif //WEMOSWEATHERSIMPLE_CONFIG_H
