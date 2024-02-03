#include "main_system.h"
#include "swerve_drive_controller.h"
#include <Arduino.h>

MainSystem mainSystem;
SwerveDriveController swerveDrive{mainSystem.driveBase};

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }
    bool result = mainSystem.begin();
    if (result) {
        mainSystem.logger.printf("Everything OK");
    } else {
        mainSystem.logger.printf("Some systems failed");
    }
    mainSystem.connection.setStatusMessageByte(2, 0x01); // Arbitrary value
}

void loop() {

    Message message;
    if (mainSystem.connection.getMessage(&message)) {
        Serial.print("Received UDP message: ");
        for (uint i = 0; i < UDP_MESSAGE_SIZE; ++i) {
            Serial.print("0x");
            Serial.print(message.data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        if (message.data[0] = 0x01) {
            SwerveDriveController::Command command;
            command.linear.x = (message.data[1] - 128.0f) / 128.0f;
            command.linear.y = (message.data[2] - 128.0f) / 128.0f;
            command.angular.z = (message.data[3] - 128.0f) / 128.0f;
            Serial.print("Command: ");
            Serial.print("x: ");
            Serial.print(command.linear.x);
            Serial.print(", y: ");
            Serial.print(command.linear.y);
            Serial.print(", z: ");
            Serial.print(command.angular.z);
            Serial.println();
            swerveDrive.setCommand(command);
        }
    }
    delay(20);
}
