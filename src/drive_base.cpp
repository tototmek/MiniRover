#include "drive_base.h"
#include "definitions.h"
#include <esp_timer.h>

void motorControlCallback(void* arg) {
    DriveBase* instance = (DriveBase*)arg;
    instance->step();
}

DriveBase::DriveBase(ServoDriver& servoDriver)
    : velocityControllers_{VelocityJointController{
                               servoDriver,
                               SERVO_CHANNEL_FRONT_RIGHT_WHEEL_VELOCITY},
                           VelocityJointController{
                               servoDriver,
                               SERVO_CHANNEL_FRONT_LEFT_WHEEL_VELOCITY},
                           VelocityJointController{
                               servoDriver,
                               SERVO_CHANNEL_REAR_LEFT_WHEEL_VELOCITY},
                           VelocityJointController{
                               servoDriver,
                               SERVO_CHANNEL_REAR_RIGHT_WHEEL_VELOCITY}},
      angleControllers_{
          AngleJointController{servoDriver,
                               SERVO_CHANNEL_FRONT_RIGHT_WHEEL_ANGLE},
          AngleJointController{servoDriver,
                               SERVO_CHANNEL_FRONT_LEFT_WHEEL_ANGLE},
          AngleJointController{servoDriver,
                               SERVO_CHANNEL_REAR_LEFT_WHEEL_ANGLE},
          AngleJointController{servoDriver,
                               SERVO_CHANNEL_REAR_RIGHT_WHEEL_ANGLE}} {}

bool DriveBase::begin() {
    esp_timer_handle_t timer;
    esp_timer_create_args_t timerArgs = {.callback = motorControlCallback,
                                         .arg = (void*)this};
    esp_timer_create(&timerArgs, &timer);
    auto result = esp_timer_start_periodic(timer, kControlPeriod);
    return (result == ESP_OK);
}

void DriveBase::step() {
    if (lastCommandTimestamp_ + kCommandTimeout < millis()) {
        stop(); // because command timeout occurred
    }
    for (uint8_t i = 0; i < kWheelCount; ++i) {
        velocityControllers_[i].step();
        angleControllers_[i].step();
    }
}

void DriveBase::setVelocity(uint8_t wheelIdx, float metersPerSecond) {
    velocityControllers_[wheelIdx].setVelocity(metersPerSecond);
    lastCommandTimestamp_ = millis();
}

void DriveBase::setAngle(uint8_t wheelIdx, float radians) {
    angleControllers_[wheelIdx].setAngle(radians);
    lastCommandTimestamp_ = millis();
}

void DriveBase::stop() {
    for (uint8_t i = 0; i < kWheelCount; ++i) {
        velocityControllers_[i].setVelocity(0.0f);
        angleControllers_[i].setAngle(0.0f);
    }
}