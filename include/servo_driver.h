#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include "definitions.h"
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

// Frequency of servo control signal (Hertz)
constexpr int kServoRate = 330;
// Minimum valid pulse value of a servo (microseconds)
constexpr int kServoMinPulseTime = 500;
// Maximum valid pulse value of a servo (microseconds)
constexpr int kServoMaxPulseTime = 2500;
// Servo control signal period (microseconds)
constexpr int kServoPeriod = 1000000 / kServoRate;
// Minimum valid pulse value of a servo
constexpr int kServoMinPulse = (1 << 12) * kServoMinPulseTime / kServoPeriod;
// Maximum valid pulse value of a servo
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