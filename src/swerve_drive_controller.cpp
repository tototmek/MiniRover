#include "swerve_drive_controller.h"

namespace {
float angleDifference(float alpha, float beta) { return beta - alpha; }
} // namespace

SwerveDriveController::SwerveDriveController(DriveBase& driveBase)
    : driveBase_(driveBase) {}

void SwerveDriveController::setCommand(TwistCommand command) {
    // Swerve drive inverse kinematics
    float A = command.linear.y - command.angular.z * kHalfWheelBase / 2.0f;
    float B = command.linear.y + command.angular.z * kHalfWheelBase / 2.0f;
    float C = command.linear.x - command.angular.z * kHalfTrackWidth / 2.0f;
    float D = command.linear.x + command.angular.z * kHalfTrackWidth / 2.0f;
    targetWheelVelocity[FRONT_RIGHT] = sqrtf(B * B + C * C);
    targetWheelVelocity[FRONT_LEFT] = sqrtf(B * B + D * D);
    targetWheelVelocity[REAR_LEFT] = sqrtf(A * A + D * D);
    targetWheelVelocity[REAR_RIGHT] = sqrtf(A * A + C * C);
    targetWheelAngle[FRONT_RIGHT] = atan2f(B, C);
    targetWheelAngle[FRONT_LEFT] = atan2f(B, D);
    targetWheelAngle[REAR_LEFT] = atan2f(A, D);
    targetWheelAngle[REAR_RIGHT] = atan2f(A, C);
    processTargetValues();
    sendTargetValues();
}

void SwerveDriveController::setCommand(WheelCommand command) {
    targetWheelVelocity[command.wheel] = command.velocity;
    targetWheelAngle[command.wheel] = command.angle;
    processTargetValues();
    sendTargetValues();
}

void SwerveDriveController::processTargetValues() {
    for (uint8_t i = 0; i < DriveBase::kWheelCount; ++i) {
        if (!flipWheelWhenOutsideLimits(i)) {
            flipWheelToReduceMovement(i);
        }

        // TODO: ensure velocities within limits, else scale down

        // TODO: scale down velocity / stop if wheel angles are not good
    }
}

bool SwerveDriveController::flipWheelWhenOutsideLimits(uint8_t wheelIdx) {
    bool flipped = false;
    if (targetWheelAngle[wheelIdx] > DRIVE_BASE_MAX_WHEEL_ANGLE) {
        targetWheelAngle[wheelIdx] -= M_PI;
        targetWheelVelocity[wheelIdx] = -targetWheelVelocity[wheelIdx];
        flipped = true;
    } else if (targetWheelAngle[wheelIdx] < DRIVE_BASE_MIN_WHEEL_ANGLE) {
        targetWheelAngle[wheelIdx] += M_PI;
        targetWheelVelocity[wheelIdx] = -targetWheelVelocity[wheelIdx];
        flipped = true;
    }
    return flipped;
}

bool SwerveDriveController::flipWheelToReduceMovement(uint8_t wheelIdx) {
    bool flipped = false;
    float difference = angleDifference(targetWheelAngle[wheelIdx],
                                       driveBase_.getAngle(wheelIdx));
    if (difference > M_PI_2 &&
        targetWheelAngle[wheelIdx] + M_PI < DRIVE_BASE_MAX_WHEEL_ANGLE) {
        targetWheelAngle[wheelIdx] += M_PI;
        targetWheelVelocity[wheelIdx] = -targetWheelVelocity[wheelIdx];
    } else if (difference < -M_PI_2 &&
               targetWheelAngle[wheelIdx] - M_PI > DRIVE_BASE_MIN_WHEEL_ANGLE) {
        targetWheelAngle[wheelIdx] -= M_PI;
        targetWheelVelocity[wheelIdx] = -targetWheelVelocity[wheelIdx];
    }
    return flipped;
}

void SwerveDriveController::sendTargetValues() {
    for (uint8_t i = 0; i < DriveBase::kWheelCount; ++i) {
        driveBase_.setVelocity(i, targetWheelVelocity[i]);
        driveBase_.setAngle(i, targetWheelAngle[i]);
    }
}