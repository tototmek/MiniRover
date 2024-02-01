#include "logger.h"

Logger::Logger(uint8_t i2cAddress, TwoWire& i2cBus)
    : display_(DISPLAY_WIDTH, DISPLAY_HEIGHT, &i2cBus),
      i2cAddress_(i2cAddress) {}

void Logger::begin() {
    bool success = display_.begin(SSD1306_SWITCHCAPVCC, i2cAddress_);
    if (!success) {
        // Could not allocate memory for the display
        for (;;)
            ;
    }
    display_.clearDisplay();
    display_.setTextSize(1);
    display_.setTextColor(SSD1306_WHITE);
    display_.display();
}

void Logger::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    display_.clearDisplay();
    ++currentLine_;
    currentLine_ %= LINES_PER_BUFFER;
    vsnprintf(buffer_[currentLine_], CHARS_PER_LINE, format, args);
    for (uint8_t line = 0; line < LINES_PER_BUFFER; ++line) {
        display_.setCursor(0, line * PIXELS_PER_LINE);
        uint8_t bufferIdx = (currentLine_ + line + 1) % LINES_PER_BUFFER;
        display_.print(buffer_[bufferIdx]);
    }
    display_.display();
    va_end(args);
}

void Logger::clear() {
    display_.clearDisplay();
    display_.display();
    currentLine_ = 0;
    // TODO: clear buffer
}