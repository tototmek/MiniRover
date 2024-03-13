#include "pan_tilt_head.h"

#include "definitions.h"
#include "drive_base.h"
#include <esp_random.h>
#include <esp_timer.h>

void headControlCallback(void* arg) {
    PanTiltHead* instance = (PanTiltHead*)arg;
    instance->step();
}

PanTiltHead::PanTiltHead(ServoDriver& servoDriver)
    : rightController_{AngleJointController{servoDriver,
                                            SERVO_CHANNEL_HEAD_RIGHT, false}},
      leftController_{
          AngleJointController{servoDriver, SERVO_CHANNEL_HEAD_LEFT, true}} {}

bool PanTiltHead::begin() {
    esp_timer_handle_t timer;
    esp_timer_create_args_t timerArgs = {.callback = headControlCallback,
                                         .arg = (void*)this};
    esp_timer_create(&timerArgs, &timer);
    auto result = esp_timer_start_periodic(timer, kControlPeriod);
    return (result == ESP_OK);
    return true;
}

void PanTiltHead::step() {
    rightController_.step();
    leftController_.step();
}

void PanTiltHead::setOrientation(float pan, float tilt) {
    rightController_.setAngle(tilt - 0.5f * pan);
    leftController_.setAngle(tilt + 0.5 * pan);
}
