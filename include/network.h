#ifndef NETWORK_H
#define NETWORK_H

#include "WiFi.h"
#include "logger.h"

constexpr uint16_t kWiFiConnectTimeout = 15000;
constexpr uint16_t kWiFiConnectRetryPeriod = 150;
constexpr uint8_t kWiFiConnectRetryIterations =
    kWiFiConnectTimeout / kWiFiConnectRetryPeriod;

constexpr int apChannel = 10;
constexpr bool apHideSsid = false;
constexpr int apMaxConnection = 5;

class Network {
  public:
    Network(Logger& logger);
    bool begin();
    uint8_t getAbsRssi() const;

  private:
    enum class State {
        INIT,        // Initial state
        AP,          // Serves as network access point
        CONNECTED,   // Connected to a network
        DISCONNECTED // Disconnected from a network
    };
    bool createWiFi();
    bool tryConnectWiFiBlocking();
    void connectWiFi();
    void initEventHandling();
    void handleWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
    State state_ = State::INIT;
    Logger& logger_;
};

#endif // NETWORK_H