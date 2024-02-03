#ifndef DRIVE_BASE_H
#define DRIVE_BASE_H

#include "joint_controller.h"

void motorControlCallback(void* arg);

class DriveBase {
  public:
    DriveBase(ServoDriver& s);
    bool begin();
    void step();
    void setVelocity(uint8_t wheelIdx, float metersPerSecond);
    void setAngle(uint8_t wheelIdx, float radians);
    void stop();
    // Number of wheels on the robot
    static constexpr uint8_t kWheelCount = 4;
    // Time between sending consecutive commands for the servos (microseconds)
    static constexpr uint64_t kControlPeriod = 1e4;
    // Time after which received commands are treated as obsolete (milliseconds)
    static constexpr uint64_t kCommandTimeout = DRIVE_BASE_COMMAND_TIMEOUT;

  private:
    uint64_t lastCommandTimestamp_ = 0;
    VelocityJointController velocityControllers_[kWheelCount];
    AngleJointController angleControllers_[kWheelCount];
};

#endif // DRIVE_BASE_H