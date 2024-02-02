#include "network.h"
#include "esp_wifi.h"
#include "wifi_config.h"
#include <WiFi.h>

constexpr int apChannel = 10;
constexpr bool apHideSsid = false;
constexpr int apMaxConnection = 5;

Network::Network(Logger& logger) : logger_(logger) {}

bool Network::begin() {
    WiFi.mode(WIFI_AP);
    bool result = WiFi.softAP(WIFI_CONFIG_SSID, WIFI_CONFIG_PASSWORD, apChannel,
                              apHideSsid, apMaxConnection);
    if (result) {
        logger_.printf("Set up %s", WIFI_CONFIG_SSID);
        const auto ip = WiFi.softAPIP();
        logger_.printf("IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    }
    return result;
}
