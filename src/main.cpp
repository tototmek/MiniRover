#include "main_system.h"
#include "swerve_drive_controller.h"
#include <Arduino.h>
#include <INA226.h>

MainSystem mainSystem;
SwerveDriveController swerveDrive{mainSystem.driveBase};
bool systemOk = false;

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
        mainSystem.logger.printf("Systems started.");
        systemOk = true;
    } else {
        mainSystem.logger.printf("Some systems failed");
    }
}

uint16_t time_temp = 0;

void loop() {
    mainSystem.connection.setStatusMessageByte(STATUS_MSG_FLAGS,
                                               (systemOk) ? 0x01 : 0xFF);
    mainSystem.connection.setStatusMessageByte(
        STATUS_MSG_POWER, mainSystem.powerManagement.getBatteryPercentage());
    mainSystem.connection.setStatusMessageByte(STATUS_MSG_RSSI,
                                               mainSystem.network.getAbsRssi());

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
            SwerveDriveController::TwistCommand command;
            command.linear.x = mapf(message.data[1], 0.0f, 256.0f,
                                    -DRIVE_BASE_MAX_VELOCITY_LINEAR_X,
                                    DRIVE_BASE_MAX_VELOCITY_LINEAR_X);
            command.linear.y = mapf(message.data[2], 0.0f, 256.0f,
                                    -DRIVE_BASE_MAX_VELOCITY_LINEAR_Y,
                                    DRIVE_BASE_MAX_VELOCITY_LINEAR_Y);
            command.angular.z = mapf(message.data[3], 0.0f, 256.0f,
                                     -DRIVE_BASE_MAX_VELOCITY_ANGULAR_Z,
                                     DRIVE_BASE_MAX_VELOCITY_ANGULAR_Z);
            swerveDrive.setCommand(command);
        }
    }

    // Random head movement
    ++time_temp;
    if (time_temp > 50) {
        Serial.println("Moving head...");
        time_temp = esp_random() % 40;
        mainSystem.head.setOrientation(
            ((float(esp_random() % 1000)) - 500.0f) / 500.0f * 1.5f,
            ((float(esp_random() % 1000)) - 500.0f) / 500.0f * 1.5f);
    }

    delay(20);
}
