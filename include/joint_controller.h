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
                            bool reverseMotor, float startPosition = 0.0f);
    void setVelocity(float metersPerSecond);

  private:
    float velocityCoefficient = 200.0f * 2.0f / DRIVE_BASE_WHEEL_DIAMETER;
    static constexpr uint16_t kZeroPwm = (1 << 12) * 1500 / kServoPeriod;
    static constexpr uint16_t kMinDeadzone = (1 << 12) * 1450 / kServoPeriod;
    static constexpr uint16_t kMaxDeadzone = (1 << 12) * 1550 / kServoPeriod;
    static constexpr uint16_t kMinPulse = (1 << 12) * 950 / kServoPeriod;
    static constexpr uint16_t kMaxPulse = (1 << 12) * 1950 / kServoPeriod;
    static constexpr float kMinVelocity = DRIVE_BASE_MIN_WHEEL_VELOCITY;
    static constexpr float kMaxVelocity = DRIVE_BASE_MAX_WHEEL_VELOCITY;
    uint16_t velocityToPwm(float metersPerSecond) const;
    bool reverseMotor_;
};

class AngleJointController : public JointController {
  public:
    AngleJointController(ServoDriver& servoDriver, uint8_t servoChannel,
                         bool reverseMotor, float startPosition = 0.0f);
    void setAngle(float radians);
    float getAngle() const;
    float getActualDampenedAngle() const;

  private:
    static constexpr uint16_t kMinPulse =
        (1 << 12) * SERVO_ANGLE_LOWER_LIMIT_PULSE_TIME / kServoPeriod;
    static constexpr uint16_t kMaxPulse =
        (1 << 12) * SERVO_ANGLE_UPPER_LIMIT_PULSE_TIME / kServoPeriod;
    static constexpr float kMinAngle = DRIVE_BASE_MIN_WHEEL_ANGLE;
    static constexpr float kMaxAngle = DRIVE_BASE_MAX_WHEEL_ANGLE;
    uint16_t angleToPwm(float radians) const;
    float pwmToAngle(uint16_t pwm) const;
    float currentCommand_;
    bool reverseMotor_;
};

#endif // JOINT_CONTROLLER_H