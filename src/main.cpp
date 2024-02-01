#include "definitions.h"
#include "logger.h"
#include "network.h"
#include "servo_driver.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

TwoWire i2cBus = TwoWire{0};
ServoDriver servo{I2C_SERVO_ADDRESS, i2cBus};
Logger logger{I2C_DISPLAY_ADDRESS, i2cBus};
Network network{logger};

void displayI2cDevices() {
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

const int delta = kServoMaxPulse - kServoMinPulse;

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ;
    }
    i2cBus.begin(21, 22);
    logger.begin();
    logger.printf("Hello, sir!");
    delay(600);
    logger.printf("System started");
    delay(300);
    displayI2cDevices();
    if (!servo.begin()) {
        logger.printf("Servo driver: off");
    } else {
        logger.printf("Servo driver: OK");
        servo.arm();
        logger.printf("Motors armed");
    }
    logger.printf("Initializing WiFi...");
    network.begin();
    delay(600);
}

int sweep = 0;
const int loop_delay = 1;

void loop() {
    for (int i = 0; i < 4096; ++i) {
        servo.setPWM(1, i);
        servo.setPWM(0, i);
        delay(loop_delay);
    }
    for (int i = 0; i < 4096; ++i) {
        servo.setPWM(1, 4096 - i);
        servo.setPWM(0, 4096 - i);
        delay(loop_delay);
    }
}
