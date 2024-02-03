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
                                                 float startPosition)
    : JointController(servoDriver, servoChannel, velocityToPwm(startPosition)) {
}

void VelocityJointController::setVelocity(float metersPerSecond) {
    pulseWidth_ = velocityToPwm(metersPerSecond);
}

uint16_t VelocityJointController::velocityToPwm(float metersPerSecond) const {
    // TODO: implement correct conversion
    return 2048;
}

AngleJointController::AngleJointController(ServoDriver& servoDriver,
                                           uint8_t servoChannel,
                                           float startPosition)
    : JointController(servoDriver, servoChannel, angleToPwm(startPosition)),
      currentCommand_(startPosition) {}

void AngleJointController::setAngle(float radians) {
    pulseWidth_ = angleToPwm(radians);
    currentCommand_ = radians;
}

float AngleJointController::getAngle() const { return currentCommand_; }

float AngleJointController::getActualDampenedAngle() const {
    return pwmToAngle(dampener_.getCurrentPoint());
}

uint16_t AngleJointController::angleToPwm(float radians) const {
    // TODO: implement correct conversion
    return 2048;
}

float AngleJointController::pwmToAngle(uint16_t pwm) const {
    // TODO: implement correct conversion
    return 0.0f;
}
