#include "WifiService.h"

#define HOSTNAME "SolarBMS"


//fucky way of creating a server
ESP8266WebServer _webserver(80);


WifiService::WifiService()
{
    _bConnected = false;
    _client = PubSubClient(_espClient);
    WiFi.scanNetworks();
   // WiFi.mode(WIFI_STA);


}
void WifiService::publish(const char* topic, const char* payload)
{
  
  if(_client.connected()) {
    _client.publish(topic, payload);
  } else {
    Serial.println("Not connected to MQTT server!");
  }
}

bool WifiService::connectWifi(const char* ssid, const char* password)
{   
    SPIFFS.begin();
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
        startWebserver();
        
    } 
    return true;
  } else {
      Serial.println("Connection to WiFi failed");
    return false;
  }
}




bool WifiService::connectMQTT(IPAddress ip, int port, const char* user, const char* password)
{
  _client.setServer(ip, port);
  _client.setCallback(onDataCb);
  if(!_client.connected()) {
    Serial.println("Connecting to MQTT server...");
    if(_client.connect(HOSTNAME, user, password)) {
      return true;
    } else {
        return false;
    }
  }
  return true;
}

bool WifiService::connectMQTT(IPAddress ip, int port)
{
  _client.setServer(ip, port);
  _client.setCallback(onDataCb);
  if(!_client.connected()) {
    Serial.println("Connecting to MQTT server...");
    if(_client.connect(HOSTNAME)) {
      return true;
    } else {
      return false;
    }
  }
  return true;
}
bool WifiService::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WifiService::isMQTTConnected() {
  return _client.connected();
}

//method to be put in loop
void WifiService::handleMQTT() {
  _client.loop();
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

/* 
------------------------------------
NON class functions - callbacks
------------------------------------
*/

void onDataCb(char* c_topic, byte* b_data, unsigned int length) {

}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  	//TODO: create an actual page
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    _webserver.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

void startWebserver() {
  _webserver.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(_webserver.uri()))                  // send it if it exists
      _webserver.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  _webserver.begin();

}

void handleClients() {
  _webserver.handleClient();
}
