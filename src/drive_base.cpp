#include "drive_base.h"
#include "definitions.h"
#include <esp_timer.h>

void motorControlCallback(void* arg) {
    DriveBase* instance = (DriveBase*)arg;
    instance->step();
}

DriveBase::DriveBase(ServoDriver& s)
    : velControllers_{JointController{s, SERVO_CHANNEL_FRONT_LEFT_WHEEL_VEL},
                      JointController{s, SERVO_CHANNEL_FRONT_RIGHT_WHEEL_VEL},
                      JointController{s, SERVO_CHANNEL_REAR_RIGHT_WHEEL_VEL},
                      JointController{s, SERVO_CHANNEL_REAR_LEFT_WHEEL_VEL}},
      angleControllers_{
          JointController{s, SERVO_CHANNEL_FRONT_LEFT_WHEEL_ANGLE},
          JointController{s, SERVO_CHANNEL_FRONT_RIGHT_WHEEL_ANGLE},
          JointController{s, SERVO_CHANNEL_REAR_RIGHT_WHEEL_ANGLE},
          JointController{s, SERVO_CHANNEL_REAR_LEFT_WHEEL_ANGLE}} {}

bool DriveBase::begin() {
    esp_timer_handle_t timer;
    esp_timer_create_args_t timerArgs = {.callback = motorControlCallback,
                                         .arg = (void*)this};
    esp_timer_create(&timerArgs, &timer);
    auto result = esp_timer_start_periodic(timer, kControlPeriod);
    return (result == ESP_OK);
}

void DriveBase::step() {
    for (uint8_t i = 0; i < kWheelCount; ++i) {
        velControllers_[i].step();
        angleControllers_[i].step();
    }
}

void DriveBase::setVelocity(uint8_t wheelIdx, uint16_t command) {
    velControllers_[wheelIdx].setPosition(command);
}

void DriveBase::setAngle(uint8_t wheelIdx, uint16_t command) {
    angleControllers_[wheelIdx].setPosition(command);
}