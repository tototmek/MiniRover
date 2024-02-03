#include "buzzer.h"

#include <Arduino.h>

Buzzer::Buzzer(uint8_t pin) : pin_(pin) {}

bool Buzzer::begin() {
    pinMode(pin_, OUTPUT);
    analogWrite(pin_, 0);
    return true;
}

void Buzzer::on() { analogWrite(pin_, kBuzzerPulseWidth); }
void Buzzer::off() { analogWrite(pin_, 0); }

void Buzzer::blockingPulse(uint32_t onTime, uint32_t offTime) {
    on();
    delay(onTime);
    off();
    delay(offTime);
}

void Buzzer::ok() {
    blockingPulse(40, 80);
    blockingPulse(40);
}

void Buzzer::wait() { blockingPulse(200); }

void Buzzer::error() {
    blockingPulse(500, 80);
    blockingPulse(500, 80);
    blockingPulse(500);
}