#include "i2c.h"

void displayI2cDevices(TwoWire& i2cBus, Logger& logger) {
    char deviceNames[22] = {' '};
    byte error, address;
    int charIdx = 0;
    logger.printf("Detected I2C chips:");
    for (address = 1; address < 127; address++) {
        i2cBus.beginTransmission(address);
        error = i2cBus.endTransmission();
        if (error == 0) {
            charIdx += sprintf(deviceNames + charIdx, "%02X ", address);
            if (charIdx >= 18) {
                charIdx = 0;
                logger.printf(deviceNames);
                memset(deviceNames, ' ', sizeof(deviceNames) - 1);
            }
        }
    }
    logger.printf(deviceNames);
}
