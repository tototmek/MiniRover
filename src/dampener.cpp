#include "dampener.h"

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