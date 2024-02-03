#include "joint_controller.h"

JointController::JointController(ServoDriver& servoDriver, uint8_t servoChannel,
                                 uint16_t startPosition)
    : servoDriver_(servoDriver), servoChannel_(servoChannel),
      pulseWidth_(startPosition), dampener_{pulseWidth_} {}

void JointController::setPosition(float position) {
    pulseWidth_ = commandToPulseWidth(position);
}

void JointController::step() {
    uint16_t position = dampener_.get_next_point(pulseWidth_);
    servoDriver_.setPWM(servoChannel_, position);
}

VelocityJointController::VelocityJointController(ServoDriver& servoDriver,
                                                 uint8_t servoChannel,
                                                 float startPosition)
    : JointController(servoDriver, servoChannel,
                      commandToPulseWidth(startPosition)) {}

uint16_t
VelocityJointController::commandToPulseWidth(float metersPerSecond) const {
    // TODO: implement correct conversion
    return 2048;
}

AngleJointController::AngleJointController(ServoDriver& servoDriver,
                                           uint8_t servoChannel,
                                           float startPosition)
    : JointController(servoDriver, servoChannel,
                      commandToPulseWidth(startPosition)) {}

uint16_t AngleJointController::commandToPulseWidth(float radians) const {
    // TODO: implement correct conversion
    return 2048;
}
