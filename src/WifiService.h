#ifndef WifiService_h
#define WifiService_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>
#include <FS.h>


class WifiService {
    public:
        WifiService();
        void sendStatus(double voltage, double current);
        bool connectWifi(const char* ssid, const char* password);
        bool connectMQTT(IPAddress ip, int port, const char* user, const char* password);
        bool connectMQTT(IPAddress ip, int port);
        bool isConnected();
        bool isMQTTConnected();
        void publish(const char* topic, const char* payload);
        void scanAndPrintNetworks();
        void handleMQTT();
    private:
        char* _aAvailabilityTopic;
        char* _aPayloadTopic;
        bool _bConnected;
        
        WiFiClient _espClient;
        PubSubClient _client;
        void _onConnectedCb();
        void _onDisconnectedCb();
        
       
        
};

void onDataCb(char* c_topic, byte* b_data, unsigned int length);
String getContentType(String filename);
bool _handleFileRead(String path);
void startWebserver();
void handleClients();
#endif