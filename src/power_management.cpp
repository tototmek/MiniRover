#include "power_management.h"
#include "definitions.h"

namespace {

uint8_t calculateBatteryPercentage(float voltage) {
    constexpr float peukertConstant = BATTERY_PEUKERT_CONSTANT;
    constexpr float peukertConstantInverse = 1.0f / peukertConstant;
    constexpr float minVoltage = BATTERY_MIN_VOLTAGE;
    constexpr float maxVoltage = BATTERY_MAX_VOLTAGE;
    voltage = std::max(minVoltage, std::min(maxVoltage, voltage));
    float remainingJuice =
        pow((voltage - minVoltage) / (maxVoltage - minVoltage),
            peukertConstantInverse);
    uint8_t percentage = remainingJuice * 100.0;
    return percentage;
}

} // namespace

PowerManagement::PowerManagement(uint8_t i2cAddress, TwoWire& i2cBus)
    : i2cAddress_{i2cAddress}, sensor_{i2cBus} {}

bool PowerManagement::begin() {
    if (!sensor_.begin(i2cAddress_)) {
        return false;
    }
    sensor_.configure(INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US,
                      INA226_SHUNT_CONV_TIME_1100US,
                      INA226_MODE_SHUNT_BUS_CONT);
    sensor_.calibrate(CURRENT_SENSOR_SHUNT_RESISTANCE,
                      CURRENT_SENSOR_MAX_RANGE_CURRENT);
    isOk_ = true;
    return true;
}

float PowerManagement::getVoltage() {
    return (isOk_) ? sensor_.readBusVoltage() : 0.0f;
}

float PowerManagement::getCurrent() {
    return (isOk_) ? sensor_.readShuntCurrent() : 0.0f;
}

float PowerManagement::getPower() {
    return (isOk_) ? sensor_.readBusPower() : 0.0f;
}

uint8_t PowerManagement::getBatteryPercentage() {
    return calculateBatteryPercentage(getVoltage());
}