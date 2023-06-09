#pragma once
#include <Arduino.h>

#if __has_include("config.h")
#include "config.h"
#endif

#ifndef I2C_ADDRESS
#define I2C_ADDRESS 0x34
#endif

#ifndef LED_ACTIVE_PIN
#define LED_ACTIVE_PIN 13
#endif

#ifndef GPS_RX_PIN
#define GPS_RX_PIN 3
#endif

#ifndef GPS_TX_PIN
#define GPS_TX_PIN 2
#endif
