#include "main_system.h"
#include <Arduino.h>

MainSystem mainSystem;

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
            mainSystem.driveBase.setVelocity(
                FRONT_LEFT, map(message.data[1], 0, 255, 0, 4095));
            mainSystem.driveBase.setAngle(
                FRONT_LEFT, map(message.data[3], 0, 255, 0, 4095));
        }
    }
    delay(20);
}
