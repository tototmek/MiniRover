#ifndef SWERVE_DRIVE_CONTROLLER_H
#define SWERVE_DRIVE_CONTROLLER_H

#include "drive_base.h"

class SwerveDriveController {
  public:
    struct TwistCommand {
        struct {
            float x = 0;
            float y = 0;
        } linear;
        struct {
            float z = 0;
        } angular;
    };
    struct WheelCommand {
        uint8_t wheel;
        float velocity;
        float angle;
    };
    SwerveDriveController(DriveBase& driveBase);
    void setCommand(TwistCommand command);
    void setCommand(WheelCommand command);
    // The distance between front and rear wheels of the robot, halved (meters)
    static constexpr float kHalfWheelBase = DRIVE_BASE_WHEEL_BASE / 2.0f;
    // The distance between left and right wheels of the robot, halved (meters)
    static constexpr float kHalfTrackWidth = DRIVE_BASE_TRACK_WIDTH / 2.0f;

  private:
    void processTargetValues();
    void sendTargetValues();
    DriveBase& driveBase_;
    float targetWheelVelocity[DriveBase::kWheelCount] = {0.0f};
    float targetWheelAngle[DriveBase::kWheelCount] = {0.0f};
};

#endif // SWERVE_DRIVE_CONTROLLER_H