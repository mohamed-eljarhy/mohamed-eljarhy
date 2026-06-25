#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>
#include <WebServer.h>

#define WIFI_SSID     "YourSSID"
#define WIFI_PASSWORD "YourPassword"

class WiFiHandler {
public:
    WebServer server;   // <-- دة المتغير الصحيح، مفيش () نهائي

    WiFiHandler();      // Constructor
    void begin();
};

#endif
