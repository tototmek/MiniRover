#ifndef DRIVE_BASE_H
#define DRIVE_BASE_H

#include "joint_controller.h"

void motorControlCallback(void* arg);

class DriveBase {
  public:
    DriveBase(ServoDriver& s);
    bool begin();
    void step();
    void setVelocity(uint8_t wheelIdx, uint16_t command);
    void setAngle(uint8_t wheelIdx, uint16_t command);
    static constexpr uint8_t kWheelCount = 4;
    static constexpr uint64_t kControlPeriod = 1e4;

  private:
    JointController velControllers_[kWheelCount];
    JointController angleControllers_[kWheelCount];
};

#endif // DRIVE_BASE_H