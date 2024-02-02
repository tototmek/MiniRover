#ifndef I2C_H
#define I2C_H

#include "Wire.h"
#include "logger.h"

void displayI2cDevices(TwoWire& i2cBus, Logger& logger);

#endif // I2C_H