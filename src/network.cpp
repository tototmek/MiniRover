#include "network.h"
#include "esp_wifi.h"
#include "wifi_config.h"
#include <WiFi.h>

Network::Network(Logger& logger) : logger_(logger) {}

bool Network::begin() {
    bool connectionSuccess = tryConnectWiFiBlocking();
    if (connectionSuccess) {
        return true;
    }
    bool result = createWiFi();
    return result;
}

bool Network::createWiFi() {
    WiFi.mode(WIFI_AP);
    bool result = WiFi.softAP(WIFI_AP_CONFIG_SSID, WIFI_AP_CONFIG_PASSWORD,
                              apChannel, apHideSsid, apMaxConnection);
    if (result) {
        state_ = State::AP;
        logger_.printf("Set up %s", WIFI_AP_CONFIG_SSID);
        const auto ip = WiFi.softAPIP();
        logger_.printf("IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    }
    return result;
}

bool Network::tryConnectWiFiBlocking() {
    connectWiFi();
    uint8_t iteration = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(kWiFiConnectRetryPeriod);
        if (++iteration >= kWiFiConnectRetryIterations) { // could not connect
            return false;
        };
    }
    state_ = State::CONNECTED;
    initEventHandling();
    logger_.printf("Connected to %s", WIFI_STA_CONFIG_SSID);
    const auto ip = WiFi.localIP();
    logger_.printf("IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return true;
}

void Network::connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_STA_CONFIG_SSID, WIFI_STA_CONFIG_PASSWORD);
};

void Network::initEventHandling() {
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        this->handleWiFiEvent(event, info);
    });
}

void Network::handleWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
    IPAddress ip;
    switch (event) {
    case SYSTEM_EVENT_STA_CONNECTED:
        state_ = State::CONNECTED;
        logger_.printf("Reconnected");
        ip = WiFi.localIP();
        logger_.printf("IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        if (state_ == State::CONNECTED) {
            state_ = State::DISCONNECTED;
            logger_.printf("WiFi lost...");
            connectWiFi();
        }
        break;
    default:
        break;
    }
}