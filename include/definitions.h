#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Pin definitions
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define SERVO_OE_PIN 19
#define BUZZER_PIN 12

// I2C device addresses
#define I2C_SERVO_ADDRESS 0x61
#define I2C_DISPLAY_ADDRESS 0x3C
#define I2C_CURRENT_SENSOR_ADDRESS 0x41

// Power parameters
#define BATTERY_MIN_VOLTAGE 6.0 // Volt
#define BATTERY_MAX_VOLTAGE 8.4 // Volt
#define BATTERY_PEUKERT_CONSTANT 1.3
#define CURRENT_SENSOR_SHUNT_RESISTANCE 0.005 // Ohm
#define CURRENT_SENSOR_MAX_RANGE_CURRENT 16.0 // Amper

// UDP ports
#define UDP_IN_PORT 21370
#define UDP_OUT_PORT 21371

// Servo channels
#define SERVO_CHANNEL_FRONT_RIGHT_WHEEL_VELOCITY 0
#define SERVO_CHANNEL_FRONT_LEFT_WHEEL_VELOCITY 1
#define SERVO_CHANNEL_REAR_LEFT_WHEEL_VELOCITY 2
#define SERVO_CHANNEL_REAR_RIGHT_WHEEL_VELOCITY 3
#define SERVO_CHANNEL_FRONT_RIGHT_WHEEL_ANGLE 4
#define SERVO_CHANNEL_FRONT_LEFT_WHEEL_ANGLE 5
#define SERVO_CHANNEL_REAR_LEFT_WHEEL_ANGLE 6
#define SERVO_CHANNEL_REAR_RIGHT_WHEEL_ANGLE 7
#define SERVO_CHANNEL_HEAD_RIGHT 12
#define SERVO_CHANNEL_HEAD_LEFT 13

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
#define DRIVE_BASE_MAX_VELOCITY_LINEAR_X 0.2   // meters/second
#define DRIVE_BASE_MAX_VELOCITY_LINEAR_Y 0.2   // meters/second
#define DRIVE_BASE_MAX_VELOCITY_ANGULAR_Z 1.0  // radians/second

// Swerve drive parameters
#define SWERVE_DRIVE_STOPPING_ANGLE_THRESHOLD 0.5 // radians

#define REVERSE_FRONT_RIGHT_WHEEL_VELOCITY true
#define REVERSE_FRONT_LEFT_WHEEL_VELOCITY false
#define REVERSE_REAR_LEFT_WHEEL_VELOCITY false
#define REVERSE_REAR_RIGHT_WHEEL_VELOCITY true
#define REVERSE_FRONT_RIGHT_WHEEL_ANGLE false
#define REVERSE_FRONT_LEFT_WHEEL_ANGLE false
#define REVERSE_REAR_LEFT_WHEEL_ANGLE false
#define REVERSE_REAR_RIGHT_WHEEL_ANGLE false

// Servo calibration
#define SERVO_MIN_PULSE_TIME 500                // microseconds
#define SERVO_MAX_PULSE_TIME 2500               // microseconds
#define SERVO_ANGLE_UPPER_LIMIT_PULSE_TIME 2500 // microseconds
#define SERVO_ANGLE_LOWER_LIMIT_PULSE_TIME 500  // microseconds

// WIFI settings
#define WIFI_CONNECT_TIMEOUT 15000 // miliseconds

// Status message byte locations
#define STATUS_MSG_FLAGS 2
#define STATUS_MSG_RSSI 1
#define STATUS_MSG_POWER 3

#endif // DEFINITIONS_H