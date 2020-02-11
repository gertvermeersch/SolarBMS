#ifndef WifiService_h
#define WifiService_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

class WifiService {
    public:
        WifiService();
        void sendStatus(double voltage, double current);
        void connectWifi(char* ssid, char* password);
        void connectMQTT(int* ip, int port, char* user, char* password);
        void publish(char* topic, char* payload);
        bool isConnected();
    private:
        char* _aAvailabilityTopic;
        char* _aPayloadTopic;
        bool _bConnected;
        WiFiClient _espClient;
        PubSubClient _client;
        
};

#endif