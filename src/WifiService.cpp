#include "WifiService.h"

#define HOSTNAME "SolarBMS"

WifiService::WifiService()
{
    _bConnected = false;
    _client = PubSubClient(_espClient);
    WiFi.mode(WIFI_STA);

}
void WifiService::sendStatus(double voltage, double current)
{
}
void WifiService::connectWifi(char *ssid, char *password)
{
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");

  // Wait for connection to wifi
  short timer = 0;
  while (WiFi.status() != WL_CONNECTED && timer++ < 100) //10 seconds timeout
  {
    
    delay(100);
    Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED) {
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  this->_bConnected = true;

    if (MDNS.begin(HOSTNAME))
    {
        Serial.println("MDNS responder started");
    } 
  } else {
      Serial.println("Connection to WiFi failed");
      this->_bConnected = false;
  }
}

void WifiService::connectMQTT(int *ip, int port, char *user, char *password)
{
}

void WifiService::publish(char *topic, char *payload)
{
}

bool WifiService::isConnected() {
    return _bConnected;
}