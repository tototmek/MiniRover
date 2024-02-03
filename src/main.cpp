#include "buzzer.h"
#include "connection.h"
#include "definitions.h"
#include "drive_base.h"
#include "i2c.h"
#include "logger.h"
#include "network.h"
#include "servo_driver.h"
#include <Arduino.h>
#include <Wire.h>

TwoWire i2cBus = TwoWire{0};
Logger logger{I2C_DISPLAY_ADDRESS, i2cBus};
ServoDriver servo{I2C_SERVO_ADDRESS, i2cBus};
DriveBase driveBase{servo};
Buzzer buzzer{BUZZER_PIN};
Network network{logger};
Connection connection{network};

void setup() {
    buzzer.begin();
    buzzer.wait();
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    i2cBus.begin(21, 22);
    logger.begin();
    displayI2cDevices(i2cBus, logger);
    if (!servo.begin()) {
        logger.printf("Servo driver: off");
    } else {
        if (driveBase.begin()) {
            logger.printf("Drive base ready");
            servo.arm();
            logger.printf("Motors armed");
        }
    }
    logger.printf("Initializing WiFi...");
    network.begin();
    connection.setStatusMessageByte(2, 0x01); // Arbitrary value
    connection.begin();
    buzzer.ok();
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
        if (message.data[0] = 0x01) {
            driveBase.setVelocity(FRONT_LEFT,
                                  map(message.data[1], 0, 255, 0, 4095));
            driveBase.setAngle(FRONT_LEFT,
                               map(message.data[3], 0, 255, 0, 4095));
        }
    }
    delay(20);
}
