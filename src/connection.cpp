#include "connection.h"
#include "definitions.h"
#include <esp_timer.h>

void statusTimerCallback(void* arg) {
    Connection* c = (Connection*)arg;
    c->sendStatus();
}

void receiveTimerCallback(void* arg) {
    Connection* c = (Connection*)arg;
    c->receive();
}

Connection::Connection(Network& network) : network_(network) {
    messageQueue_ = xQueueCreate(UDP_MESSAGE_QUEUE_SIZE, sizeof(Message));
}

bool Connection::begin() {
    udpInputPort_.begin(UDP_IN_PORT);
    udpOutputPort_.begin(UDP_OUT_PORT);
    esp_timer_create_args_t statusTimerArgs = {
        .callback = statusTimerCallback,
        .arg = (void*)this,
    };
    esp_timer_create(&statusTimerArgs, &statusTimer_);
    esp_timer_create_args_t receiveTimerArgs = {
        .callback = receiveTimerCallback,
        .arg = (void*)this,
    };
    esp_timer_create(&receiveTimerArgs, &receiveTimer_);
    bool resultB =
        esp_timer_start_periodic(statusTimer_, kStatusTimerPeriod) == ESP_OK;
    bool resultR =
        esp_timer_start_periodic(receiveTimer_, kReceiveTimerPeriod) == ESP_OK;
    return (resultB && resultR);
}

bool Connection::getMessage(Message* dest) {
    return (xQueueReceive(messageQueue_, (void*)dest, portMAX_DELAY) == pdPASS);
}

void Connection::setStatusMessageByte(uint8_t byteLocation, uint8_t value) {
    statusData_.data[byteLocation] = value;
}

void Connection::sendStatus() {
    udpOutputPort_.beginPacket("255.255.255.255", UDP_OUT_PORT);
    udpOutputPort_.write(statusData_.data, sizeof(statusData_.data));
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
