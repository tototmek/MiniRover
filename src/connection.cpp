#include "connection.h"
#include "definitions.h"
#include <esp_timer.h>

void broadcastTimerCallback(void* arg) {
    Connection* c = (Connection*)arg;
    c->broadcast();
}

void receiveTimerCallback(void* arg) {
    Connection* c = (Connection*)arg;
    c->receive();
}

Connection::Connection() {
    messageQueue_ = xQueueCreate(UDP_MESSAGE_QUEUE_SIZE, sizeof(Message));
}

bool Connection::begin() {
    udpInputPort_.begin(UDP_IN_PORT);
    udpOutputPort_.begin(UDP_OUT_PORT);
    esp_timer_create_args_t broadcastTimerArgs = {
        .callback = broadcastTimerCallback,
        .arg = (void*)this,
    };
    esp_timer_create(&broadcastTimerArgs, &broadcastTimer_);
    esp_timer_create_args_t receiveTimerArgs = {
        .callback = receiveTimerCallback,
        .arg = (void*)this,
    };
    esp_timer_create(&receiveTimerArgs, &receiveTimer_);
    bool resultB = esp_timer_start_periodic(broadcastTimer_,
                                            kBroadcastTimerPeriod) == ESP_OK;
    bool resultR =
        esp_timer_start_periodic(receiveTimer_, kReceiveTimerPeriod) == ESP_OK;
    return (resultB == ESP_OK && resultR == ESP_OK);
}

void Connection::broadcast() {
    udpOutputPort_.beginPacket("255.255.255.255", UDP_OUT_PORT);
    udpOutputPort_.write(broadcastData_.data, sizeof(broadcastData_.data));
    udpOutputPort_.endPacket();
}

void Connection::receive() {
    int packetSize = udpInputPort_.parsePacket();
    if (packetSize == UDP_MESSAGE_SIZE) {
        Message message;
        udpInputPort_.read(message.data, packetSize);
        xQueueSend(messageQueue_, &message, portMAX_DELAY);
    }
}

bool Connection::getMessage(Message* dest) {
    return (xQueueReceive(messageQueue_, (void*)dest, portMAX_DELAY) == pdPASS);
}

void Connection::setBroadcastMessage(const Message* data) {
    for (uint i = 0; i < UDP_MESSAGE_SIZE; ++i) {
        broadcastData_.data[i] = data->data[i];
    }
}