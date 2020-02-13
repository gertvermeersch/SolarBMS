#include <Arduino.h>
#include <SPI.h>
#include "SolarBMS.h"
#include "pass.h"
#include "WifiService.h"
#include <Ticker.h>


#define RELAY_PIN 13

Ticker ticker;
double voltage = 0;
double current = 0;

IPAddress ip = IPAddress(192,168,1,14);

SolarBMS bms(RELAY_PIN, 4, 5, 0x48);
WifiService wifiService;
short timer = 0;
bool send = false;

void setup();

void loop();
void onTimerInterrupt();

void setup()
{

  pinMode(RELAY_PIN, OUTPUT);
  
  Serial.begin(74880);
  Serial.println("SolarBMS v0.3");
  wifiService = WifiService();
  wifiService.scanAndPrintNetworks();
  if(wifiService.connectWifi(ssid, password)) {
    wifiService.connectMQTT(ip, 1883, mqtt_user, mqtt_pass);
  }
  ticker.attach(60, onTimerInterrupt);

}

void loop()
{
  
  delay(100); //F*ck around a little bit...

  //TODO: test library stability - take out incorrect reading

  if(!wifiService.isConnected()) {
    if(wifiService.connectWifi(ssid, password)) {
      wifiService.connectMQTT(ip, 1883, mqtt_user, mqtt_pass);
    }
  }
   if(wifiService.isConnected() && send) {
     Serial.print("Voltage: "); //TODO: this can be written shorter snprintf
    Serial.print(voltage);
    Serial.print("\tCurrent: ");
    Serial.println(current, 4);
    bms.determineRelay(voltage);
      wifiService.sendStatus(voltage, current);
      send = !send;
      Serial.println("MQTT message published");
  } else {
      handleClients();
  }
  

}

void onTimerInterrupt() {
  voltage = bms.readVoltage();
  current = bms.readCurrent();
  
  send = true; //send in the main loop

}

