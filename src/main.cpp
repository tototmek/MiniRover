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
            command.linear.x = map(message.data[1], 0, 256, 0, 4096);
            command.linear.y = 0;
            command.angular.z = map(message.data[3], 0, 256, 0, 4096);
            swerveDrive.setCommand(command);
        }
    }
    delay(20);
}
