#include "joint_controller.h"

JointController::JointController(ServoDriver& servoDriver, uint8_t servoChannel,
                                 uint16_t startPosition)
    : servoDriver_(servoDriver), servoChannel_(servoChannel),
      pulseWidth_(startPosition), dampener_{pulseWidth_} {}

void JointController::step() {
    uint16_t position = dampener_.getNextPoint(pulseWidth_);
    servoDriver_.setPWM(servoChannel_, position);
}

VelocityJointController::VelocityJointController(ServoDriver& servoDriver,
                                                 uint8_t servoChannel,
                                                 bool reverseMotor,
                                                 float startPosition)
    : JointController(servoDriver, servoChannel, velocityToPwm(startPosition)),
      reverseMotor_(reverseMotor) {}

void VelocityJointController::setVelocity(float metersPerSecond) {
    if (reverseMotor_) {
        metersPerSecond = -metersPerSecond;
    }
    pulseWidth_ = velocityToPwm(metersPerSecond);
}

uint16_t VelocityJointController::velocityToPwm(float metersPerSecond) const {
    // TODO: implement correct conversion
    return 2048;
}

AngleJointController::AngleJointController(ServoDriver& servoDriver,
                                           uint8_t servoChannel,
                                           bool reverseMotor,
                                           float startPosition)
    : JointController(servoDriver, servoChannel, angleToPwm(startPosition)),
      currentCommand_(startPosition), reverseMotor_(reverseMotor) {}

void AngleJointController::setAngle(float radians) {
    currentCommand_ = radians;
    if (reverseMotor_) {
        radians = -radians;
    }
    pulseWidth_ = angleToPwm(radians);
}

float AngleJointController::getAngle() const { return currentCommand_; }

float AngleJointController::getActualDampenedAngle() const {
    return pwmToAngle(dampener_.getCurrentPoint());
}

uint16_t AngleJointController::angleToPwm(float radians) const {
    // TODO: test the calibration
    float t = (radians - kMinAngle) / (kMaxAngle - kMinAngle);
    uint16_t pwm = kMinPulse + uint16_t(float(kMaxPulse - kMinPulse) * t);
    return pwm;
}

float AngleJointController::pwmToAngle(uint16_t pwm) const {
    float t = float(pwm - kMinPulse) / float(kMaxPulse - kMinPulse);
    float radians = kMinAngle + (kMaxAngle - kMinAngle) * t;
    return radians;
}
