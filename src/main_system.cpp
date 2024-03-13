#include "main_system.h"
#include "definitions.h"

MainSystem::MainSystem()
    : buzzer{BUZZER_PIN}, i2cBus{0}, logger{I2C_DISPLAY_ADDRESS, i2cBus},
      powerManagement{I2C_CURRENT_SENSOR_ADDRESS, i2cBus},
      servoDriver{I2C_SERVO_ADDRESS, i2cBus}, driveBase{servoDriver},
      head{servoDriver}, network{logger}, connection{network} {}

bool MainSystem::begin() {
    bool powerManagementOk, servoDriverOk, driveBaseOk, headOk, networkOk,
        connectionOk = false;
    buzzer.begin();
    buzzer.wait();
    i2cBus.begin(21, 22);
    logger.begin();
    logger.printf("Initializing system");
    displayI2cDevices(i2cBus, logger);
    powerManagementOk = powerManagement.begin();
    if (powerManagementOk) {
        logger.printf("Battery level %d%%",
                      powerManagement.getBatteryPercentage());
    } else {
        logger.printf("Battery level unknown");
    }
    servoDriverOk = servoDriver.begin();
    if (servoDriverOk) {
        driveBaseOk = driveBase.begin();
        if (driveBaseOk) {
            logger.printf("Drive base ready");
            servoDriver.arm();
            logger.printf("Motors armed");
        }
        headOk = head.begin();
    } else {
        logger.printf("Servo driver: off");
    }
    logger.printf("Initializing WiFi...");
    networkOk = network.begin();
    connectionOk = connection.begin();
    bool result = (powerManagementOk && servoDriverOk && driveBaseOk &&
                   headOk && networkOk && connectionOk);
    if (result) {
        buzzer.ok();
    } else {
        buzzer.error();
    }
    return result;
}
