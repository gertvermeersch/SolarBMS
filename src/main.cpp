#include <Arduino.h>
#include <SPI.h>
#include "SolarBMS.h"
#include "pass.h"
#include "WifiService.h"
#include <ArduinoJson.h>
#include <Ticker.h>

#define AVAILABLE_TOPIC "/solar/available"
#define SENSOR_TOPIC "/solar/sensor"

#define RELAY_PIN 13

Ticker ticker;
double meanVoltage = 0;
double meanHour = 0;
double current = 0;
char buffer[100];
double secondsVoltage[6];
//double minuteCurrent[6];
double minutesVoltage[60];
short secondscounter;
short minutescounter;
short hourscounter;

IPAddress ip = IPAddress(192, 168, 1, 14);

SolarBMS bms;
WifiService wifiService;

short timer = 0;
bool send = true;

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
  Serial.println("SolarBMS v0.3");
  Serial.setDebugOutput(true);
  bms = SolarBMS(RELAY_PIN, 4, 5, 0x48);
  wifiService = WifiService();
  wifiService.addWifiNetwork(ssid, password);
  wifiService.connectWifi();
  ticker.attach(10, onTimerInterrupt);
}

void loop()
{
  //TODO: test library stability - take out incorrect reading

  // maintainWifi();
  maintainMQTT();

  // handleClients();
  wifiService.handleMQTT();
  handleClients();
  //dont forget to add mqtt client looping here if required
  delay(500);

  if (secondscounter == 6)
  {
    secondscounter = 0;
    meanVoltage = 0;
    for (short i = 0; i < 6; i++)
    {
      meanVoltage += secondsVoltage[i];
    }
    meanVoltage = meanVoltage / 6;
    getValueJSON();
    bms.determineRelay(meanVoltage);
    Serial.println(minutescounter);
    Serial.println(buffer);
    minutesVoltage[minutescounter++] = meanVoltage;
  }

  if(minutescounter == 60) {
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

void maintainWifi()
{
  //maintain wifi connection
  if (!wifiService.isConnected())
  {
    Serial.println("Wifi connection lost!");
    if (wifiService.connectWifi())
    {
      Serial.println("Connection to WiFi restored");
    }
  }
}

void maintainMQTT()
{
  //maintain mqtt connection
  if (wifiService.isConnected() && !wifiService.isMQTTConnected())
  {
    Serial.println("MQTT connection lost, reconnecting...");
    if (wifiService.connectMQTT(ip, 1883))
    {
      Serial.println("MQTT connection restored");
    }
    else
    {
      Serial.println("MQTT connection failed!");
    }
  }
}

void onTimerInterrupt()
{ //each 10 seconds
  bms.readCurrent();
  bms.readVoltage();
  secondsVoltage[secondscounter++] = bms.getLastVoltage();
  
  // wifiService.publish(SENSOR_TOPIC, buffer);
}

void getValueJSON()
{
  //TODO": Check efficiency of library
  const int capacity = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<capacity> doc;
  doc["voltage"] = meanVoltage;
  doc["current"] = bms.getLastCurrent();
  serializeJson(doc, buffer);
}

void sendStatus()
{
  wifiService.publish(AVAILABLE_TOPIC, "online");
}
