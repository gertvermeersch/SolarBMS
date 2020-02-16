#include <Arduino.h>
#include <SPI.h>
#include "SolarBMS.h"
#include "pass.h"
#include "WifiService.h"
#include <Ticker.h>

#define AVAILABLE_TOPIC "/solar/available"
#define SENSOR_TOPIC "/solar/sensor"

#define RELAY_PIN 13

Ticker ticker;
double voltage = 0;
double current = 0;
char buffer[100];

IPAddress ip = IPAddress(192,168,1,14);

SolarBMS bms(RELAY_PIN, 4, 5, 0x48);
WifiService wifiService;
short timer = 0;
bool send = false;

void setup();
void getValueJSON(char* buffer);
void loop();
void onTimerInterrupt();
void sendStatus();

void setup()
{

  pinMode(RELAY_PIN, OUTPUT);
  
  Serial.begin(74880);
  Serial.println("SolarBMS v0.3");
  wifiService = WifiService();
  wifiService.scanAndPrintNetworks();
  if(wifiService.connectWifi(ssid, password)) {
    wifiService.connectMQTT(ip, 1883);
  }
  ticker.attach(60, onTimerInterrupt);

}

void loop()
{
  delay(50);
  bms.readVoltage();
  delay(50);
  bms.readCurrent();


  //TODO: test library stability - take out incorrect reading

  if(!wifiService.isConnected()) {
    if(wifiService.connectWifi(ssid, password)) {
      wifiService.connectMQTT(ip, 1883);
    }
  }
   if(wifiService.isConnected() && send) {
    
    getValueJSON(buffer);
    const char* payload = buffer;
    bms.determineRelay(voltage);
    wifiService.publish(SENSOR_TOPIC, payload);
    send = !send;
    Serial.println("MQTT message published");
  } else {
      handleClients();
  }
  

}

void onTimerInterrupt() {
  send = true;

}

void getValueJSON(char* buffer) {
  //TODO": Check efficiency of library
  const int capacity = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<capacity> doc;
  doc["voltage"] = bms.getLastVoltage();
  doc["current"] = bms.getLastCurrent();
  doc["voltageRaw"] = bms.getLastVoltageRaw();
  doc["currentRaw"] = bms.getLastCurrentRaw();
  serializeJson(doc, buffer);
  Serial.println(buffer);
}

void sendStatus() {
  wifiService.publish(AVAILABLE_TOPIC, "online");
}

