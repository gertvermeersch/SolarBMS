#include <Arduino.h>
#include <SPI.h>
#include "SolarBMS.h"
#include "WifiService.h"
#include "pass.h"

#define RELAY_PIN 13


const char* mqtt_server = "192.168.1.14";

SolarBMS bms(RELAY_PIN, 4, 5, 0x48);
WifiService wifiService;


void setup();

void loop();


void setup()
{

  pinMode(RELAY_PIN, OUTPUT);
  
  Serial.begin(115200);
  Serial.println("SolarBMS v0.3");
  wifiService = WifiService();
  wifiService.connectWifi(ssid, password);


}

void loop()
{
  bms.cycleRelay();
  delay(100);

  Serial.print("Voltage: ");
  Serial.print(bms.readVoltage());
  delay(100);

  Serial.print("\tCurrent: ");
  Serial.println(bms.readCurrent(), 4);

  delay(500);

  if(!wifiService.isConnected()) {
    wifiService.connectWifi(ssid, password);
  }
}

