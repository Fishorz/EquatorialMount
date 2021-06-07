#pragma once
//
#define BAUD_RATE 115200
//IR Pin
#define irLED 8

// Stepper Motor Pin Define
#define Dir_Pin 6
#define Step_Pin 5

// Encoder Pin Define
#define outputA 3
#define outputB 2
#define Button 7

// LED
// #define PIN_LED 4

//Limit the Exponsure time and Interval Time
#define Max_Time_Limit 200
#define Min_Time_Limit 0

//TMC2209 Pin Setup
#define STALL_VALUE 50 // [0... 255]
#define TOFF_VALUE 4   // [1... 15]

#define EN_PIN 11 // Enable pin
#define SW_RX 10 // SoftwareSerial receive pin
#define SW_TX 9  // SoftwareSerial transmit pin
//#define SERIAL_PORT Serial1 // HardwareSerial port
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f       // Match to your driver
#define RMS 400             //rms_current
#define MICROSTEPS 256      //MICROSTEPS 1-256