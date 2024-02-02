#include "connection.h"
#include "definitions.h"
#include "logger.h"
#include "network.h"
#include "servo_driver.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <Wire.h>

TwoWire i2cBus = TwoWire{0};
ServoDriver servo{I2C_SERVO_ADDRESS, i2cBus};
Logger logger{I2C_DISPLAY_ADDRESS, i2cBus};
Network network{logger};
Connection connection;

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
    Message broadcastMessage = {.data = {0x01, 0x01, 0x01, 0x01}};
    connection.setBroadcastMessage(&broadcastMessage);
    connection.begin();
}

void loop() {
    Message message;
    if (connection.getMessage(&message)) {
        Serial.print("Received UDP message: ");
        for (uint i = 0; i < UDP_MESSAGE_SIZE; ++i) {
            Serial.print("0x");
            Serial.print(message.data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
    delay(30);
}
