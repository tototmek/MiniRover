#include "swerve_drive_controller.h"

SwerveDriveController::SwerveDriveController(DriveBase& driveBase)
    : driveBase_(driveBase) {}

void SwerveDriveController::setCommand(Command command) {
    // TODO: Swerve drive inverse kinematics should be implemented here
    for (uint8_t i = 0; i < DriveBase::kWheelCount; ++i) {
        driveBase_.setVelocity(i, command.linear.x);
        driveBase_.setAngle(i, command.angular.z);
    }
}