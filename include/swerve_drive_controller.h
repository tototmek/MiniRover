#ifndef SWERVE_DRIVE_CONTROLLER_H
#define SWERVE_DRIVE_CONTROLLER_H

#include "drive_base.h"

class SwerveDriveController {
  public:
    struct Command {
        struct {
            float x = 0;
            float y = 0;
        } linear;
        struct {
            float z = 0;
        } angular;
    };
    SwerveDriveController(DriveBase& driveBase);
    void setCommand(Command command);
    static constexpr float kHalfWheelBase = DRIVE_BASE_WHEEL_BASE / 2.0f;
    static constexpr float kHalfTrackWidth = DRIVE_BASE_TRACK_WIDTH / 2.0f;

  private:
    DriveBase& driveBase_;
};

#endif // SWERVE_DRIVE_CONTROLLER_H