#ifndef JOINT_CONTROLLER_H
#define JOINT_CONTROLLER_H

#include "dampener.h"
#include "servo_driver.h"
#include <cstdint>

class JointController {
  public:
    JointController(ServoDriver& servoDriver, uint8_t servoChannel,
                    uint16_t startPosition);
    void setPosition(float command);
    void step();

  protected:
    virtual uint16_t commandToPulseWidth(float command) const = 0;

  private:
    ServoDriver& servoDriver_;
    const uint8_t servoChannel_;
    uint16_t pulseWidth_;
    Dampener dampener_;
};

class VelocityJointController : public JointController {
  public:
    VelocityJointController(ServoDriver& servoDriver, uint8_t servoChannel,
                            float startPosition = 0.0f);

  protected:
    uint16_t commandToPulseWidth(float metersPerSecond) const override;
};

class AngleJointController : public JointController {
  public:
    AngleJointController(ServoDriver& servoDriver, uint8_t servoChannel,
                         float startPosition = 0.0f);

  protected:
    uint16_t commandToPulseWidth(float radians) const override;
};

#endif // JOINT_CONTROLLER_H