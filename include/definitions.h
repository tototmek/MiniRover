#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Pin definitions
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define SERVO_OE_PIN 19
#define BUZZER_PIN 12

// I2C device addresses
#define I2C_SERVO_ADDRESS 0x40
#define I2C_DISPLAY_ADDRESS 0x3C

// UDP ports
#define UDP_IN_PORT 21370
#define UDP_OUT_PORT 21371

// Status message byte locations
#define STATUS_MSG_FLAGS 1
#define STATUS_MSG_POWER 2
#define STATUS_MSG_RSSI 3

// Servo channels
#define SERVO_CHANNEL_FRONT_RIGHT_WHEEL_VELOCITY 0
#define SERVO_CHANNEL_FRONT_LEFT_WHEEL_VELOCITY 1
#define SERVO_CHANNEL_REAR_LEFT_WHEEL_VELOCITY 2
#define SERVO_CHANNEL_REAR_RIGHT_WHEEL_VELOCITY 3
#define SERVO_CHANNEL_FRONT_RIGHT_WHEEL_ANGLE 4
#define SERVO_CHANNEL_FRONT_LEFT_WHEEL_ANGLE 5
#define SERVO_CHANNEL_REAR_LEFT_WHEEL_ANGLE 6
#define SERVO_CHANNEL_REAR_RIGHT_WHEEL_ANGLE 7

// Wheel indices
#define FRONT_RIGHT 0
#define FRONT_LEFT 1
#define REAR_LEFT 2
#define REAR_RIGHT 3

// Drive base parameters
#define DRIVE_BASE_COMMAND_TIMEOUT 2000        // milliseconds
#define DRIVE_BASE_WHEEL_BASE 0.32             // meters
#define DRIVE_BASE_TRACK_WIDTH 0.24            // meters
#define DRIVE_BASE_WHEEL_DIAMETER 0.096        // meters
#define DRIVE_BASE_MAX_WHEEL_VELOCITY 0.5      // meters/second
#define DRIVE_BASE_MIN_WHEEL_VELOCITY -0.5     // meters/second
#define DRIVE_BASE_MAX_WHEEL_ANGLE 2.61799388  // radians
#define DRIVE_BASE_MIN_WHEEL_ANGLE -2.61799388 // radians
#define DRIVE_BASE_MAX_VELOCITY_LINEAR_X 0.5   // meters/second
#define DRIVE_BASE_MAX_VELOCITY_LINEAR_Y 0.5   // meters/second
#define DRIVE_BASE_MAX_VELOCITY_ANGULAR_Z 0.5  // radians/second

#endif // DEFINITIONS_H