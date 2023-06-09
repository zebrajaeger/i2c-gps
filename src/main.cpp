
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <Wire.h>

#include "default_config.h"
#include "timer.h"
#include "types.h"
#include "wireutils.h"

// -----------------------------------------------------------------------------
#define FLAGS_LOCATION_BIT 1
#define FLAGS_ALTITUDE_BIT 2
#define FLAGS_DATE_BIT 4
#define FLAGS_TIME_BIT 8
#define FLAGS_SATELITES_BIT 16
#define FLAGS_HDOP_BIT 32

// -----------------------------------------------------------------------------
void displayInfo();
void requestEvent();

// -----------------------------------------------------------------------------
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);
TinyGPSPlus gps;
class GPSTimer : public IntervalTimer {
  virtual void onTimer() {
    displayInfo();
    digitalWrite(LED_ACTIVE_PIN, gps.location.isValid());
  }
};
GPSTimer gpsTimer;

// -----------------------------------------------------------------------------
void setup()
// -----------------------------------------------------------------------------
{
  Serial.begin(115200);
  ss.begin(9600);

  // LEDs
  pinMode(LED_ACTIVE_PIN, OUTPUT);
  digitalWrite(LED_ACTIVE_PIN, false);

  // I²C
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(requestEvent);

  gpsTimer.start(1000, false);
}

// -----------------------------------------------------------------------------
void loop()
// -----------------------------------------------------------------------------
{
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }
  gpsTimer.loop();
}

// -----------------------------------------------------------------------------
void requestEvent()
// -----------------------------------------------------------------------------
{
  // 1 + (12) + (3) + (3) + (4) + (4) + 2 = 1 + (28) = 29
  u8_t t8;
  u16_t t16;
  u32_t t32;

  u8_t valid;
  valid.uint8 = 0;
  u8_t updated;
  updated.uint8 = 0;

  // Latitude/Longitude = 8Bytes
  if (gps.location.isValid()) {
    valid.uint8 |= FLAGS_LOCATION_BIT;
  }
  if (gps.location.isUpdated()) {
    updated.uint8 |= FLAGS_LOCATION_BIT;
  }
  t32.float32 = gps.location.lat();
  WireUtils::write32(t32);
  t32.float32 = gps.location.lng();
  WireUtils::write32(t32);

  // Altitude = 4
  if (gps.altitude.isValid()) {
    valid.uint8 |= FLAGS_ALTITUDE_BIT;
  }
  if (gps.altitude.isUpdated()) {
    updated.uint8 |= FLAGS_ALTITUDE_BIT;
  }
  t32.float32 = gps.altitude.meters();
  WireUtils::write32(t32);

  // Date = 3
  if (gps.date.isValid()) {
    valid.uint8 |= FLAGS_DATE_BIT;
  }
  if (gps.date.isUpdated()) {
    updated.uint8 |= FLAGS_DATE_BIT;
  }
  t8.uint8 = gps.date.year();
  WireUtils::write16(t16);
  t8.uint8 = gps.date.month();
  WireUtils::write8(t8);
  t8.uint8 = gps.date.day();
  WireUtils::write8(t8);

  // Time = 3
  if (gps.time.isValid()) {
    valid.uint8 |= FLAGS_TIME_BIT;
  }
  if (gps.time.isUpdated()) {
    updated.uint8 |= FLAGS_TIME_BIT;
  }
  t8.uint8 = gps.time.hour();
  WireUtils::write8(t8);
  t8.uint8 = gps.time.minute();
  WireUtils::write8(t8);
  t8.uint8 = gps.time.second();
  WireUtils::write8(t8);

  // Satelites = 4
  if (gps.satellites.isValid()) {
    valid.uint8 |= FLAGS_SATELITES_BIT;
  }
  if (gps.satellites.isUpdated()) {
    updated.uint8 |= FLAGS_SATELITES_BIT;
  }
  t32.uint32 = gps.satellites.value();
  WireUtils::write32(t32);

  // HDOP = 4
  if (gps.hdop.isValid()) {
    valid.uint8 |= FLAGS_HDOP_BIT;
  }
  if (gps.hdop.isUpdated()) {
    updated.uint8 |= FLAGS_HDOP_BIT;
  }
  t32.uint32 = gps.hdop.value();
  WireUtils::write32(t32);

  // Flags = 2
  WireUtils::write8(valid);
  WireUtils::write8(updated);
}

void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" SAT:"));
  if (gps.satellites.isValid()) {
    Serial.print(gps.satellites.value());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" HDOP:"));
  if (gps.hdop.isValid()) {
    Serial.print(gps.hdop.value());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
