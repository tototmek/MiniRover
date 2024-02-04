#ifndef MAIN_SYSTEM_H
#define MAIN_SYSTEM_H

#include "buzzer.h"
#include "connection.h"
#include "drive_base.h"
#include "i2c.h"
#include "logger.h"
#include "network.h"
#include "power_management.h"
#include "servo_driver.h"
#include <Wire.h>

class MainSystem {
  public:
    MainSystem();
    bool begin();
    Buzzer buzzer;
    TwoWire i2cBus;
    Logger logger;
    PowerManagement powerManagement;
    ServoDriver servoDriver;
    DriveBase driveBase;
    Network network;
    Connection connection;
};

#endif // MAIN_SYSTEM_H