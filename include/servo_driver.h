#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include "definitions.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

constexpr int kServoRate = 330;                    // Hz
constexpr int kServoMinPulseTime = 500;            // us
constexpr int kServoMaxPulseTime = 2500;           // us
constexpr int kServoPeriod = 1000000 / kServoRate; // us
constexpr int kServoMinPulse = (1 << 12) * kServoMinPulseTime / kServoPeriod;
constexpr int kServoMaxPulse = (1 << 12) * kServoMaxPulseTime / kServoPeriod;

class ServoDriver {
  public:
    ServoDriver(uint8_t i2cAddress, TwoWire& i2cBus);
    bool begin();
    void setPWM(uint8_t channel, uint16_t pwm);
    void arm();
    void disarm();

  private:
    Adafruit_PWMServoDriver driver_;
};

#endif // SERVO_DRIVER_H