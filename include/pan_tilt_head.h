#ifndef PAN_TILT_HEAD_H
#define PAN_TILT_HEAD_H

#include "joint_controller.h"

void headControlCallback(void* arg);

class PanTiltHead {
  public:
    PanTiltHead(ServoDriver& s);
    bool begin();
    void step();
    void setOrientation(float pan, float tilt);
    // Time between sending consecutive commands for the servos (microseconds)
    static constexpr uint64_t kControlPeriod = 1e4;

  private:
    AngleJointController rightController_;
    AngleJointController leftController_;
};

#endif // PAN_TILT_HEAD_H