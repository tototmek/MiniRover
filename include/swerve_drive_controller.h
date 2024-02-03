#ifndef SWERVE_DRIVE_CONTROLLER_H
#define SWERVE_DRIVE_CONTROLLER_H

#include "drive_base.h"

class SwerveDriveController {
  public:
    struct Command {
        struct {
            uint16_t x = 0;
            uint16_t y = 0;
        } linear;
        struct {
            uint16_t z = 0;
        } angular;
    };
    SwerveDriveController(DriveBase& driveBase);
    void setCommand(Command command);

  private:
    DriveBase& driveBase_;
};

#endif // SWERVE_DRIVE_CONTROLLER_H