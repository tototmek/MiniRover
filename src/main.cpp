#include "buzzer.h"
#include "connection.h"
#include "definitions.h"
#include "i2c.h"
#include "joint_controller.h"
#include "logger.h"
#include "network.h"
#include "servo_driver.h"
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <esp_timer.h>

TwoWire i2cBus = TwoWire{0};
Logger logger{I2C_DISPLAY_ADDRESS, i2cBus};
ServoDriver servo{I2C_SERVO_ADDRESS, i2cBus};
JointController velocityController{servo, 0};
JointController angleController{servo, 1};
Buzzer buzzer{BUZZER_PIN};
Network network{logger};
Connection connection;

void motorControlCallback(void* arg);

void setup() {
    buzzer.begin();
    buzzer.blockingPulse(90);
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    i2cBus.begin(21, 22);
    logger.begin();
    logger.printf("Hello, sir!");
    delay(600);
    logger.printf("System started");
    delay(300);
    displayI2cDevices(i2cBus, logger);
    if (!servo.begin()) {
        logger.printf("Servo driver: off");
    } else {
        logger.printf("Servo driver: OK");
        servo.arm();
        logger.printf("Motors armed");
        buzzer.blockingPulse(30, 60);
        buzzer.blockingPulse(30);
    }
    logger.printf("Initializing WiFi...");
    network.begin();
    Message broadcastMessage = {.data = {0x01, 0x01, 0x01, 0x01}};
    connection.setBroadcastMessage(&broadcastMessage);
    connection.begin();
    esp_timer_handle_t timer;
    esp_timer_create_args_t timerArgs = {.callback = motorControlCallback,
                                         .arg = (void*)0};
    esp_timer_create(&timerArgs, &timer);
    esp_timer_start_periodic(timer, 1e4);
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
            velocityController.setPosition(
                map(message.data[1], 0, 255, 0, 4095));
            angleController.setPosition(map(message.data[3], 0, 255, 0, 4096));
        }
    }
    delay(30);
}

void motorControlCallback(void* arg) {
    angleController.step();
    velocityController.step();
}
