#ifndef CONNECTION_H
#define CONNECTION_H

#include "WiFiUdp.h"
#include <freertos/queue.h>

#define UDP_MESSAGE_SIZE 4
#define UDP_MESSAGE_QUEUE_SIZE 5

constexpr uint64_t kBroadcastTimerPeriod = 1e6; // 1000000 us = 1 s
constexpr uint64_t kReceiveTimerPeriod = 1e4;   // 10000 us = 10 ms

void broadcastTimerCallback(void* arg);
void receiveTimerCallback(void* arg);

struct Message {
    uint8_t data[UDP_MESSAGE_SIZE];
};

class Connection {
  public:
    Connection();
    bool begin();
    void setBroadcastMessage(const Message* data);
    bool getMessage(Message* dest);
    void broadcast();
    void receive();

  private:
    WiFiUDP udpOutputPort_;
    WiFiUDP udpInputPort_;
    Message broadcastData_;
    QueueHandle_t messageQueue_;
    esp_timer_handle_t broadcastTimer_;
    esp_timer_handle_t receiveTimer_;
};

#endif // CONNECTION_H