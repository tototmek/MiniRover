#ifndef BUZZER_H
#define BUZZER_H

#include <cstdint>

constexpr uint8_t kBuzzerPulseWidth = 80;

class Buzzer {
  public:
    Buzzer(uint8_t pin);
    bool begin();
    void on();
    void off();
    void blockingPulse(uint32_t onTime, uint32_t offTime = 0);
    void ok();
    void wait();
    void error();

  private:
    uint8_t pin_;
};

#endif // BUZZER_H