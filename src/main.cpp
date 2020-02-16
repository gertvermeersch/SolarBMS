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
double voltage = 0;
double current = 0;
char* buffer;

IPAddress ip = IPAddress(192,168,1,14);

SolarBMS bms(RELAY_PIN, 4, 5, 0x48);
WifiService wifiService;
short timer = 0;
bool send = true;

void setup();
void getValueJSON(char* buffer);
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
  wifiService = WifiService();
  wifiService.scanAndPrintNetworks();
  if(wifiService.connectWifi(ssid, password)) {
    if(wifiService.connectMQTT(ip, 1883)) {
        sendStatus();
        wifiService.publish(SENSOR_TOPIC, "test");
      } else {
        Serial.println("MQTT connection failed!");
      }
  }
  ticker.attach(10, onTimerInterrupt);

}

void loop()
{
  delay(50);
  bms.readVoltage();
  delay(50);
  bms.readCurrent();
  

  //TODO: test library stability - take out incorrect reading
   if(wifiService.isConnected() && wifiService.isMQTTConnected() && send) {
    getValueJSON(buffer);
    const char* payload = buffer;
    bms.determineRelay(voltage);
    wifiService.publish(SENSOR_TOPIC, payload);
    send = !send;
    Serial.println("MQTT message published");
  } 
  maintainWifi();
  maintainMQTT();
      
  handleClients();
  wifiService.handleMQTT();
}

void maintainWifi() {
//maintain wifi connection
  if(!wifiService.isConnected()) {
    Serial.println("Wifi connection lost!");
    if(wifiService.connectWifi(ssid, password)) {
      if(wifiService.connectMQTT(ip, 1883)) {
        sendStatus();
        wifiService.publish(SENSOR_TOPIC, "test");
      } else {
        Serial.println("MQTT connection failed!");
      }
    }
  }
}

void maintainMQTT(){
//maintain mqtt connection
  if(wifiService.isConnected() && !wifiService.isMQTTConnected()) {
    Serial.println("MQTT connection lost!");
    if(wifiService.connectMQTT(ip, 1883)) {
        sendStatus();
        wifiService.publish(SENSOR_TOPIC, "test");
      } else {
        Serial.println("MQTT connection failed!");
      }
  }
}

void onTimerInterrupt() {
  send = true;
  sendStatus();
  wifiService.publish(SENSOR_TOPIC, "test");
}

void getValueJSON(char* buffer) {
  //TODO": Check efficiency of library
  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> doc;
  doc["voltage"] = bms.getLastVoltage();
  doc["current"] = bms.getLastCurrent();
  doc["voltageRaw"] = bms.getLastVoltageRaw();
  doc["currentRaw"] = bms.getLastCurrentRaw();
  char temp[100];
  serializeJson(doc, temp);
  buffer = temp;
  Serial.println(buffer);
}

void sendStatus() {
  wifiService.publish(AVAILABLE_TOPIC, "online");
}


