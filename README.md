## Setup

1. Setup variables in [conf.h](./conf.h) file
2. [...]

#### Pins

humidy sensor:
-VCC = 3.3V
-GND = GND
-SDA = A4 (use inline 330 ohm resistor if your board is 5V)
-SCL = A5 (use inline 330 ohm resistor if your board is 5V)

spray relay: pin 8

## Sensors

#### SHT20

```
Hardware Connections:
    -VCC = 3.3V
    -GND = GND
    -SDA = A4 (use inline 330 ohm resistor if your board is 5V)
    -SCL = A5 (use inline 330 ohm resistor if your board is 5V)
```

#### ESP8266 Module

[How to connect](https://www.instructables.com/ESP-12E-ESP8266-With-Arduino-Uno-Getting-Connected/)

## Repositories

- [DFRobot_SHT20](https://github.com/DFRobot/DFRobot_SHT20)
- [Neotimer](https://github.com/jrullan/neotimer)
