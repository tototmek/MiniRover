#include "servo_driver.h"

ServoDriver::ServoDriver(uint8_t i2cAddress, TwoWire& i2cBus)
    : driver_{i2cAddress, i2cBus} {}

bool ServoDriver::begin() {
    pinMode(SERVO_OE_PIN, OUTPUT);
    digitalWrite(SERVO_OE_PIN, HIGH);
    bool success = driver_.begin();
    if (!success) {
        return false;
    }
    driver_.setOscillatorFrequency(27000000);
    driver_.setPWMFreq(kServoRate);
    return true;
}

void ServoDriver::setPWM(uint8_t channel, uint16_t pwm) {
    if (pwm > kServoMaxPulse) {
        pwm = kServoMaxPulse;
    } else if (pwm < kServoMinPulse) {
        pwm = kServoMinPulse;
    }
    driver_.setPWM(channel, 0, pwm);
}

void ServoDriver::arm() { digitalWrite(SERVO_OE_PIN, LOW); }

void ServoDriver::disarm() { digitalWrite(SERVO_OE_PIN, HIGH); }