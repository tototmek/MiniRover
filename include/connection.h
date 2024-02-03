#ifndef CONNECTION_H
#define CONNECTION_H

#include "WiFiUdp.h"
#include "network.h"
#include <freertos/queue.h>

#define UDP_MESSAGE_SIZE 4
#define UDP_MESSAGE_QUEUE_SIZE 5

constexpr uint64_t kStatusTimerPeriod = 1e6;  // 1000000 us = 1 s
constexpr uint64_t kReceiveTimerPeriod = 1e4; // 10000 us = 10 ms

void statusTimerCallback(void* arg);
void receiveTimerCallback(void* arg);

struct Message {
    uint8_t data[UDP_MESSAGE_SIZE];
};

class Connection {
  public:
    Connection(Network& network);
    bool begin();
    bool getMessage(Message* dest);
    void setStatusMessageByte(uint8_t byteLocation, uint8_t value);
    void sendStatus();
    void receive();

  private:
    Network& network_;
    WiFiUDP udpOutputPort_;
    WiFiUDP udpInputPort_;
    Message statusData_ = {0x01, 0x00, 0x00, 0x00};
    QueueHandle_t messageQueue_;
    esp_timer_handle_t statusTimer_;
    esp_timer_handle_t receiveTimer_;
};

#endif // CONNECTION_H