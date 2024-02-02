#include "joint_controller.h"

namespace {
inline uint16_t min(uint16_t lhs, uint16_t rhs) {
    return (lhs < rhs) ? lhs : rhs;
}
} // namespace

uint16_t Dampener::get_next_point(uint16_t setpoint) {
    if ((direction && setpoint > point) || (!direction && setpoint < point)) {
        // Moving in correct direction
        uint16_t accelerate_velocity = velocity + 1;
        uint16_t distance =
            (setpoint > point) ? setpoint - point : point - setpoint;
        uint16_t decelerate_velocity = kDampenerDecelerateLut[distance];
        velocity = min(accelerate_velocity, decelerate_velocity);
    } else {
        // Moving in wrong direction. Decelerate, then change direction
        if (velocity > 0) {
            velocity -= 1;
        } else {
            direction = !direction;
        }
    }

    if (direction) {
        point += velocity;
    } else {
        point -= velocity;
    }

    return point;
}

JointController::JointController(ServoDriver& servoDriver, uint8_t servoChannel,
                                 uint16_t start_position)
    : servoDriver_(servoDriver), servoChannel_(servoChannel),
      dampener_(start_position), command_(start_position) {}

void JointController::setPosition(uint16_t position) { command_ = position; }

void JointController::step() {
    uint16_t position = dampener_.get_next_point(command_);
    servoDriver_.setPWM(servoChannel_, position);
}