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
void WifiService::connectWifi(const char* ssid, const char* password)
{   
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");

  // Wait for connection to wifi
  short timer = 0;
  while (WiFi.status() != WL_CONNECTED && timer++ < 10) //10 seconds timeout
  {
    delay(1000);
    Serial.print(".");
  }
  if(WiFi.status() == WL_CONNECTED) {
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

    if (MDNS.begin(HOSTNAME))
    {
        Serial.println("MDNS responder started");
    } 
  } else {
      Serial.println("Connection to WiFi failed");
    
  }
}

void WifiService::connectMQTT(int *ip, int port, char *user, char *password)
{
}

void WifiService::publish(char *topic, char *payload)
{
}

bool WifiService::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WifiService::scanAndPrintNetworks() {
      WiFi.disconnect();
    // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
}