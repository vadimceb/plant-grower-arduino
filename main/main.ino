#include <neotimer.h>
#include <stdio.h>
#include <Wire.h>
#include "DFRobot_SHT20.h"
#include "WiFiEsp.h"
#include "conf.h"
#include "Wire.h"

#define TCAADDR 0x70

// Timers
Neotimer sprayTimer = Neotimer();
Neotimer readTimer = Neotimer();
Neotimer relayTimer = Neotimer();

// Sensors
DFRobot_SHT20 sht20;

// Helps to select i2c board
void tcaselect(uint8_t i)
{
    if (i > 7)
        return;

    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission();
}

void scanI2CPorts()
{
    Serial.println("Scan i2c channels");
    for (uint8_t t = 0; t < 8; t++)
    {
        tcaselect(t);
        Serial.print("TCA Port #");
        Serial.println(t);

        for (uint8_t addr = 0; addr <= 127; addr++)
        {
            if (addr == TCAADDR)
                continue;

            Wire.beginTransmission(addr);
            if (!Wire.endTransmission())
            {
                Serial.print("Found I2C 0x");
                Serial.println(addr, HEX);
            }
        }
    }
    Serial.println("\ndone");
}

void initSensors()
{
    // Setup connectors for data sensors
    /** Hardware Connections:
     * -VCC = 3.3V
     * -GND = GND
     * -SDA = A4 (use inline 330 ohm resistor if your board is 5V)
     * -SCL = A5 (use inline 330 ohm resistor if your board is 5V)
     */

    int size = sizeof SHT20_SENSORS_CH / sizeof *SHT20_SENSORS_CH;
    for (int i = 0; i < size; i++)
    {
        Serial.println("INIT SHT20: " + String(i));
        tcaselect(SHT20_SENSORS_CH[i]);
        sht20.initSHT20();  // Init SHT20 Sensor
        sht20.checkSHT20(); // Check SHT20 Sensor
    }
}

void setup()
{

    while (!Serial)
    {
    }

    delay(1000);

    Wire.begin();

    Serial.begin(115200);
    Serial.println();

    // Setup timers for spraying and reading data
    readTimer.set(READ_DATA_TIME);
    if (ACTIVATE_SPRAY)
    {
        sprayTimer.set(SPRAY_INTERVAL);
    }

    initSensors();

    // Setup relays
    pinMode(RELAY_PIN_1, OUTPUT);
    pinMode(RELAY_PIN_2, OUTPUT);
}

void loop()
{
    if (ACTIVATE_SPRAY)
    {
        if (sprayTimer.repeat())
        {
            spray();
        }

        if (relayTimer.done())
        {
            stopSpray();
        }
    }

    if (readTimer.repeat())
    {
        Serial.println(">> Sensor SHT20");
        int size = sizeof SHT20_SENSORS_CH / sizeof *SHT20_SENSORS_CH;
        for (int i = 0; i < size; i++)
        {
            int ch = SHT20_SENSORS_CH[i];
            tcaselect(ch);
            readSensorsData(ch);
        }
    }
}

void spray()
{
    digitalWrite(RELAY_PIN_1, HIGH);
    digitalWrite(RELAY_PIN_2, HIGH);
    relayTimer.set(SPRAY_TIME_DURATION);
    relayTimer.start();
}

void stopSpray()
{
    relayTimer.stop();
    relayTimer.reset();
    digitalWrite(RELAY_PIN_1, LOW);
    digitalWrite(RELAY_PIN_2, LOW);
}

void readSensorsData(int i)
{
    float temp = readTemperature();
    float humd = readHumidity();
    Serial.println("data" + String(i) + "|t" + String(temp) + ";h" + String(humd) + ";");
    // sendData(0, humd);
}

float readTemperature()
{
    float temp = sht20.readTemperature(); // Read Temperature
    return temp;
}

float readHumidity()
{
    float humd = sht20.readHumidity(); // Read Humidity
    return humd;
}
