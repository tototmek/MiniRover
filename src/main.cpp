#include "main_system.h"
#include "swerve_drive_controller.h"
#include <Arduino.h>

MainSystem mainSystem;
SwerveDriveController swerveDrive{mainSystem.driveBase};

float mapf(float x, float inMin, float inMax, float outMin, float outMax) {
    return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

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
            SwerveDriveController::WheelCommand command;
            command.wheel = FRONT_RIGHT;
            command.velocity = mapf(message.data[1], 0.0f, 256.0f,
                                    DRIVE_BASE_MIN_WHEEL_VELOCITY,
                                    DRIVE_BASE_MAX_WHEEL_VELOCITY);
            command.angle =
                mapf(message.data[3], 0.0f, 256.0f, DRIVE_BASE_MIN_WHEEL_ANGLE,
                     DRIVE_BASE_MAX_WHEEL_ANGLE);
            Serial.print("Command: ");
            Serial.print(command.velocity);
            Serial.print(", ");
            Serial.println(command.angle);
            swerveDrive.setCommand(command);
        }
    }
    delay(20);
}
