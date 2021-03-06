#include <Arduino.h>
#include <SPI.h>
#include "SolarBMS.h"
#include "pass.h"
#include "WifiService.h"
#include <ArduinoJson.h>
#include <Ticker.h>

#define AVAILABLE_TOPIC "solar/available"
#define SENSOR_TOPIC "solar/sensor"

#define RELAY_PIN 13

Ticker ticker;
double meanVoltage = 0;
double meanHour = 0;
double current = 0;
char buffer[100];
double secondsVoltage[60];
//double minuteCurrent[6];
double minutesVoltage[60];
short secondscounter;
short minutescounter;
short hourscounter;
bool send = false;

IPAddress ip = IPAddress(192, 168, 1, 14);

SolarBMS bms(RELAY_PIN, 4, 5, 0x48);
WifiService wifiService(ssid, password);

//short timer = 0;

void setup();
void getValueJSON();
void loop();
void onTimerInterrupt();
void sendStatus();
void maintainWifi();
void maintainMQTT();

void setup()
{

  pinMode(RELAY_PIN, OUTPUT);

  Serial.begin(74880);
  Serial.println("SolarBMS v0.4");
  Serial.setDebugOutput(true);
  ticker.attach(1, onTimerInterrupt);
  wifiService.connectWifi();
  bms.begin();
}

void loop()
{
  //maintainMQTT();
  handleClients();
  if (send)
  {
    send = false;
    getValueJSON();
    maintainMQTT();
    wifiService.publish(SENSOR_TOPIC, buffer);
    Serial.println(buffer);
  }
  delay(500);
}

void maintainMQTT()
{
  //maintain mqtt connection
  if (wifiService.isConnected() && !wifiService.isMQTTConnected())
  {
    Serial.println("(Re)connecting to mqtt broker...");
    if (wifiService.connectMQTT(ip, 1883))
    {
      wifiService.publish(AVAILABLE_TOPIC, "online");
      Serial.println("MQTT connection restored");
    }
    else
    {
      Serial.println("MQTT connection failed!");
    }
  }
  else if (!wifiService.isConnected())
  {
    wifiService.connectWifi();
  }
}

void onTimerInterrupt()
{ //each 10 seconds
  //bms.readCurrent();
  //bms.readVoltage();
  secondsVoltage[secondscounter++] = bms.getLastVoltage();

  if (secondscounter == 60)
  {
    secondscounter = 0;
    meanVoltage = 0;

    for (short i = 0; i < 60; i++)
    {
      meanVoltage += secondsVoltage[i];
    }
    meanVoltage = roundf(meanVoltage / 60 * 100) / 100;
    bms.determineRelay(meanVoltage);
    send = true;

    Serial.println(minutescounter);
    

    minutesVoltage[minutescounter++] = meanVoltage;
  }

  if (minutescounter == 60)
  {
    minutescounter = 0;
    meanHour = 0;
    for (short i = 0; i < 60; i++)
    {
      meanHour += minutesVoltage[i];
    }
    meanHour = meanHour / 60;
    Serial.print("average for the hour: ");
    Serial.println(meanHour);
  }
}

void getValueJSON()
{
  //TODO": Check efficiency of library
  const int capacity = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<capacity> doc;
  doc["voltage"] = meanVoltage;
  doc["current"] = bms.getLastCurrent();
  doc["power_enabled"] = bms.getRelayOn();
  serializeJson(doc, buffer);
}

void sendStatus()
{
  wifiService.publish(AVAILABLE_TOPIC, "online");
}
