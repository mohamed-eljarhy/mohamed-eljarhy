#include "WiFiHandler.h"

WiFiHandler::WiFiHandler()
: server(80)   // <-- التهيئة الصح
{
}

void WiFiHandler::begin() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
    }

    server.begin();   // <-- هنا بقى object مش function
}
