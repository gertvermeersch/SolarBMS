#include "WifiService.h"

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