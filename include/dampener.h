#ifndef DAMPENER_H
#define DAMPENER_H

#include <cstdint>

static constexpr uint8_t kDampenerDecelerateLut[4096] = {
#include "dampener_decelerate_lut.dat"
};

class Dampener {
  public:
    Dampener(uint16_t start_point = 0) : point(start_point) {}
    uint16_t getNextPoint(uint16_t setpoint);
    uint16_t getCurrentPoint() const;

  private:
    bool direction = false;
    uint16_t point = 0;
    uint16_t velocity = 0;
};

#endif // DAMPENER_H