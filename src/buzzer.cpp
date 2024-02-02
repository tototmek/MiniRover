#include "buzzer.h"

#include <Arduino.h>

Buzzer::Buzzer(uint8_t pin) : pin_(pin) {}

bool Buzzer::begin() {
    pinMode(pin_, OUTPUT);
    digitalWrite(pin_, LOW);
    return true;
}

void Buzzer::on() { digitalWrite(pin_, HIGH); }
void Buzzer::off() { digitalWrite(pin_, LOW); }

void Buzzer::blockingPulse(uint32_t onTime, uint32_t offTime) {
    on();
    delay(onTime);
    off();
    delay(offTime);
}
