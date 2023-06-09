# i2c GPS

## Hardware
- Arduino Nano (Uno, ...)
- GP-02 GPS-Module

### Wiring

|Arduino | GP-02 |
|---|---|
|3(RX)|TX|
|2(TX)*|RX*|
|5V|5V|
|GND|GND|

\* Optional

## I²C
- Address: 0x34

### Data (29 bytes)
- float32: Location.Latitude
- float32: Location.Longitude
- float32: Altitude
- uint16: Date.Year
- uint8: Date.Month
- uint8: Date.Day
- uint8: Date.Hour
- uint8: Date.Minute
- uint8: Date.Second
- uint32: Satelites
- uint32: Hdop
- uint8: valid flags
- uint8: updated flags (since last request)

## Flags (valid & updated)

- FLAGS_LOCATION_BIT 1
- FLAGS_ALTITUDE_BIT 2
- FLAGS_DATE_BIT 4
- FLAGS_TIME_BIT 8
- FLAGS_SATELITES_BIT 16
- FLAGS_HDOP_BIT 32


    