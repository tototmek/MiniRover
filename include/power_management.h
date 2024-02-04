#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H
#include <Wire.h>

#include <INA226.h>

class PowerManagement {
  public:
    PowerManagement(uint8_t i2cAddress, TwoWire& i2cBus);
    bool begin();
    float getVoltage();
    float getCurrent();
    float getPower();
    uint8_t getBatteryPercentage();

  private:
    bool isOk_ = false;
    uint8_t i2cAddress_;
    INA226 sensor_;
};

#endif // POWER_MANAGEMENT_H