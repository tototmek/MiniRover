#ifndef JOINT_CONTROLLER_H
#define JOINT_CONTROLLER_H

#include "dampener.h"
#include "servo_driver.h"
#include <cstdint>

class JointController {
  public:
    JointController(ServoDriver& servoDriver, uint8_t servoChannel,
                    uint16_t startPosition);
    void step();

  protected:
    uint16_t pulseWidth_;
    Dampener dampener_;

  private:
    ServoDriver& servoDriver_;
    const uint8_t servoChannel_;
};

class VelocityJointController : public JointController {
  public:
    VelocityJointController(ServoDriver& servoDriver, uint8_t servoChannel,
                            float startPosition = 0.0f);
    void setVelocity(float metersPerSecond);

  private:
    uint16_t velocityToPwm(float metersPerSecond) const;
};

class AngleJointController : public JointController {
  public:
    AngleJointController(ServoDriver& servoDriver, uint8_t servoChannel,
                         float startPosition = 0.0f);
    void setAngle(float radians);
    float getAngle() const;
    float getActualDampenedAngle() const;

  private:
    uint16_t angleToPwm(float radians) const;
    float pwmToAngle(uint16_t pwm) const;
    float currentCommand_;
};

#endif // JOINT_CONTROLLER_H