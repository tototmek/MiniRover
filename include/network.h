#ifndef NETWORK_H
#define NETWORK_H

#include "WiFi.h"
#include "logger.h"

class Network {
  public:
    Network(Logger& logger);
    bool begin();

  private:
    Logger& logger_;
};

#endif // NETWORK_H