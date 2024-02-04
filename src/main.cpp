#include "main_system.h"
#include "swerve_drive_controller.h"
#include <Arduino.h>
// #include <INA226_WE.h>
#include <INA226.h>

MainSystem mainSystem;
SwerveDriveController swerveDrive{mainSystem.driveBase};
// INA226_WE sensorMeasurement{I2C_CURRENT_SENSOR_ADDRESS};
INA226 currentMeasurement{mainSystem.i2cBus};

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
    if (currentMeasurement.begin(I2C_CURRENT_SENSOR_ADDRESS)) {
        mainSystem.logger.printf("C sensor initialized");
    } else {
        mainSystem.logger.printf("C sensor error");
    }
    currentMeasurement.configure(INA226_AVERAGES_1, INA226_BUS_CONV_TIME_1100US,
                                 INA226_SHUNT_CONV_TIME_1100US,
                                 INA226_MODE_SHUNT_BUS_CONT);
    currentMeasurement.calibrate(0.1, 0.8);
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
            command.wheel = FRONT_LEFT;
            swerveDrive.setCommand(command);
            command.wheel = REAR_LEFT;
            swerveDrive.setCommand(command);
            command.wheel = REAR_RIGHT;
            swerveDrive.setCommand(command);
        }
    }
    Serial.println("Power measurements:");
    float voltage = currentMeasurement.readBusVoltage();
    float current = currentMeasurement.readShuntCurrent();
    float power = currentMeasurement.readBusPower();
    Serial.print("Voltage: ");
    Serial.print(voltage, 3);
    Serial.print(", Current: ");
    Serial.print(current, 3);
    Serial.print(", Power: ");
    Serial.print(power, 3);
    Serial.println();
    delay(20);
}
