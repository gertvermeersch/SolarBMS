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
#define DEBUG_WIFI 1

Ticker ticker;
double voltage = 0;
double current = 0;
char buffer[100];

IPAddress ip = IPAddress(192,168,1,14);

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
  bms = SolarBMS(RELAY_PIN, 4, 5, 0x48);
  wifiService = WifiService();
  ticker.attach(10, onTimerInterrupt);
}

void loop()
{
  //TODO: test library stability - take out incorrect reading
  
  maintainWifi();
  maintainMQTT();
      
  handleClients();
  //wifiService.handleMQTT();
}

void maintainWifi() {
//maintain wifi connection
  if(!wifiService.isConnected()) {
    Serial.println("Wifi connection lost!");
    if(wifiService.connectWifi(ssid, password)) {
      Serial.println("Connection to WiFi restored");
    }
  }
}

void maintainMQTT(){
//maintain mqtt connection
  if(wifiService.isConnected() && !wifiService.isMQTTConnected()) {
    Serial.println("MQTT connection lost!");
    if(wifiService.connectMQTT(ip, 1883)) {
        Serial.println("MQTT connection restored");
      } else {
        Serial.println("MQTT connection failed!");
      }
  }
}

void onTimerInterrupt() {
    bms.readCurrent();
    bms.readVoltage();
    getValueJSON();
    bms.determineRelay(voltage);
    Serial.println(buffer);
    wifiService.publish(SENSOR_TOPIC, buffer);
}

void getValueJSON() {
  //TODO": Check efficiency of library
  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> doc;
  doc["voltage"] = bms.getLastVoltage();
  doc["current"] = bms.getLastCurrent();
  doc["voltageRaw"] = bms.getLastVoltageRaw();
  doc["currentRaw"] = bms.getLastCurrentRaw();
  serializeJson(doc, buffer);
}

void sendStatus() {
  wifiService.publish(AVAILABLE_TOPIC, "online");
}


