#include "main_system.h"
#include "definitions.h"

MainSystem::MainSystem()
    : buzzer{BUZZER_PIN}, i2cBus{0}, logger{I2C_DISPLAY_ADDRESS, i2cBus},
      servoDriver{I2C_SERVO_ADDRESS, i2cBus}, driveBase{servoDriver},
      network{logger}, connection{network} {}

bool MainSystem::begin() {
    bool servoDriverOk, driveBaseOk, networkOk, connectionOk = false;
    buzzer.begin();
    buzzer.wait();
    i2cBus.begin(21, 22);
    logger.begin();
    logger.printf("Initializing system");
    displayI2cDevices(i2cBus, logger);
    servoDriverOk = servoDriver.begin();
    if (servoDriverOk) {
        driveBaseOk = driveBase.begin();
        if (driveBaseOk) {
            logger.printf("Drive base ready");
            servoDriver.arm();
            logger.printf("Motors armed");
        }
    } else {
        logger.printf("Servo driver: off");
    }
    logger.printf("Initializing WiFi...");
    networkOk = network.begin();
    connectionOk = connection.begin();
    bool result = (servoDriverOk && driveBaseOk && networkOk && connectionOk);
    if (result) {
        buzzer.ok();
    } else {
        buzzer.error();
    }
    return result;
}