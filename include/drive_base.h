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

  private:
    JointController velControllers_[4];
    JointController angleControllers_[4];
};

#endif // DRIVE_BASE_H