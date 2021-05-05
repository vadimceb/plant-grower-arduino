#include <neotimer.h>
#include <stdio.h>
#include <Wire.h>
#include "DFRobot_SHT20.h"
#include "WiFiEsp.h"
#include "conf.h"

// Timers
Neotimer sprayTimer = Neotimer();
Neotimer readTimer = Neotimer();
Neotimer relayTimer = Neotimer();

// Sensors
DFRobot_SHT20 sht20;

// WIFI
// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

int status = WL_IDLE_STATUS; // the Wifi radio's status
WiFiEspClient client;        // Initialize the Ethernet client object

void setup()
{
    Serial.begin(115200);
    Serial.println();

    // Setup timers for spraying and reading data
    sprayTimer.set(SPRAY_INTERVAL);
    readTimer.set(READ_DATA_TIME);

    // Setup connectors for data sensors
    /** Hardware Connections:
     * -VCC = 3.3V
     * -GND = GND
     * -SDA = A4 (use inline 330 ohm resistor if your board is 5V)
     * -SCL = A5 (use inline 330 ohm resistor if your board is 5V)
     */
    sht20.initSHT20(); // Init SHT20 Sensor
    delay(100);
    sht20.checkSHT20(); // Check SHT20 Sensor

    // Setup relays
    pinMode(RELAY_PIN, OUTPUT);
}

void setupWifi()
{
    // initialize ESP module
    WiFi.init(&Serial1);

    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("WiFi shield not present");
        // don't continue
        while (true)
        {
        }
    }

    // attempt to connect to WiFi network
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }

    Serial.println("You're connected to the network");
    printWifiData();
}

void printWifiData()
{
    // print your WiFi shield's IP address
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}

void loop()
{
    if (sprayTimer.repeat())
    {
        spray();
    }

    if (readTimer.repeat())
    {
        readSensorsData();
    }

    if (relayTimer.done())
    {
        stopSpray();
    }
}

void spray()
{
    //TODO: to be implemented..
    // 1. Activate relays
    digitalWrite(RELAY_PIN, LOW);
    relayTimer.set(500);
    relayTimer.start();
}

void stopSpray()
{
    relayTimer.stop();
    relayTimer.reset();
    digitalWrite(RELAY_PIN, HIGH);
}

void readSensorsData()
{
    float temp = readTemperature();
    float humd = readHumidity();

    sendData(temp, humd);
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

bool sendData(float temp, float humd)
{
    int currTime = millis();

    //TODO: to be implemented
    if (client.connect(SERVER_TO_SEND_DATA, 80))
    {
        Serial.println("Connected to server");
        // Make a HTTP request
        client.println("GET /asciilogo.txt HTTP/1.1");
        client.println("Host: arduino.cc");
        client.println("Connection: close");
        client.println();
    }
}