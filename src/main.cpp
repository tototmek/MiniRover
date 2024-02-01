#include "definitions.h"
#include "logger.h"
#include "network.h"
#include "servo_driver.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <esp_timer.h>

TwoWire i2cBus = TwoWire{0};
ServoDriver servo{I2C_SERVO_ADDRESS, i2cBus};
Logger logger{I2C_DISPLAY_ADDRESS, i2cBus};
Network network{logger};
WiFiUDP udpOutputPort;
WiFiUDP udpInputPort;

void timerCallback(void* arg);

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
    udpInputPort.begin(21370);
    udpOutputPort.begin(21371);
    // Initialize timer
    esp_timer_create_args_t timerArgs = {
        .callback = &timerCallback, .arg = (void*)0, .name = "network_timer"};
    esp_timer_handle_t timer;
    esp_timer_create(&timerArgs, &timer);
    esp_timer_start_periodic(timer, 1e6);
}

void loop() {
    int packetSize = udpInputPort.parsePacket();
    if (packetSize == 4) {
        uint8_t message[4];
        udpInputPort.read(message, packetSize);
        if (message[0] == 0x01) { // Servo command
            servo.setPWM(0, map(message[1], 0, 263, 0, 4095));
            servo.setPWM(1, map(message[2], 0, 255, 0, 4095));
        }
    }
    delay(5);
}

void timerCallback(void* arg) {
    byte buffer[] = {0x01, 0x01, 0x01, 0x42};
    udpOutputPort.beginPacket("255.255.255.255", 21371);
    udpOutputPort.write(buffer, sizeof(buffer));
    udpOutputPort.endPacket();
}