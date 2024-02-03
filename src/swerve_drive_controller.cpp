#include "swerve_drive_controller.h"

SwerveDriveController::SwerveDriveController(DriveBase& driveBase)
    : driveBase_(driveBase) {}

void SwerveDriveController::setCommand(Command command) {
    // // Swerve drive inverse kinematics
    float A = command.linear.y - command.angular.z * kHalfWheelBase / 2.0f;
    float B = command.linear.y + command.angular.z * kHalfWheelBase / 2.0f;
    float C = command.linear.x - command.angular.z * kHalfTrackWidth / 2.0f;
    float D = command.linear.x + command.angular.z * kHalfTrackWidth / 2.0f;
    driveBase_.setVelocity(FRONT_RIGHT, sqrtf(B * B + C * C));
    driveBase_.setVelocity(FRONT_LEFT, sqrtf(B * B + D * D));
    driveBase_.setVelocity(REAR_LEFT, sqrtf(A * A + D * D));
    driveBase_.setVelocity(REAR_RIGHT, sqrtf(A * A + C * C));
    driveBase_.setAngle(FRONT_RIGHT, atan2f(B, C));
    driveBase_.setAngle(FRONT_LEFT, atan2f(B, D));
    driveBase_.setAngle(REAR_LEFT, atan2f(A, D));
    driveBase_.setAngle(REAR_RIGHT, atan2f(A, C));
}