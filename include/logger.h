#ifndef LOGGER_H
#define LOGGER_H

#include "Wire.h"
#include <Adafruit_SSD1306.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define PIXELS_PER_LINE 9
#define CHARS_PER_LINE 22
#define LINES_PER_BUFFER 7

class Logger {
  public:
    Logger(uint8_t i2cAddress, TwoWire& i2cBus);
    void begin();
    void printf(const char* format, ...);
    void clear();

  private:
    void printAtLine(uint8_t line, const char* text);
    Adafruit_SSD1306 display_;
    uint8_t i2cAddress_;
    char buffer_[LINES_PER_BUFFER][CHARS_PER_LINE] = {" "};
    uint8_t currentLine_ = 0;
};

#endif // LOGGER_H