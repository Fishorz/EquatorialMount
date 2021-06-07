#pragma once
#include <TMCStepper.h>
#include "ConfigFile.h"

// Select your stepper driver type
//TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
TMC2209Stepper driver(SW_RX, SW_TX, R_SENSE, DRIVER_ADDRESS);
using namespace TMC2209_n;
void TMCstepperSetup()
{
    // Serial.begin(115200); // Init serial port and set baudrate
    while (!Serial)
        ; // Wait for serial port to connect
    Serial.println("\nStart...");

    //SERIAL_PORT.begin(115200);
    driver.beginSerial(BAUD_RATE);
    driver.begin();

    // Sets the slow decay time (off time) [1... 15]. This setting also limits
    // the maximum chopper frequency. For operation with StealthChop,
    // this parameter is not used, but it is required to enable the motor.
    // In case of operation with StealthChop only, any setting is OK.
    driver.toff(TOFF_VALUE);

    // VACTUAL allows moving the motor by UART control.
    // It gives the motor velocity in +-(2^23)-1 [μsteps / t]
    // 0: Normal operation. Driver reacts to STEP input.
    // /=0: Motor moves with the velocity given by VACTUAL.
    // Step pulses can be monitored via INDEX output.
    // The motor direction is controlled by the sign of VACTUAL.
    // driver.VACTUAL(speed);

    // Comparator blank time. This time needs to safely cover the switching
    // event and the duration of the ringing on the sense resistor. For most
    // applications, a setting of 16 or 24 is good. For highly capacitive
    // loads, a setting of 32 or 40 will be required.
    driver.blank_time(24);

    driver.rms_current(RMS); // mA
    driver.microsteps(MICROSTEPS);

    // Lower threshold velocity for switching on smart energy CoolStep and StallGuard to DIAG output
    driver.TCOOLTHRS(0xFFFFF); // 20bit max

    // CoolStep lower threshold [0... 15].
    // If SG_RESULT goes below this threshold, CoolStep increases the current to both coils.
    // 0: disable CoolStep
    driver.semin(5);

    // CoolStep upper threshold [0... 15].
    // If SG is sampled equal to or above this threshold enough times,
    // CoolStep decreases the current to both coils.
    driver.semax(2);

    // Sets the number of StallGuard2 readings above the upper threshold necessary
    // for each current decrement of the motor current.
    driver.sedn(0b01);

    // StallGuard4 threshold [0... 255] level for stall detection. It compensates for
    // motor specific characteristics and controls sensitivity. A higher value gives a higher
    // sensitivity. A higher value makes StallGuard4 more sensitive and requires less torque to
    // indicate a stall. The double of this value is compared to SG_RESULT.
    // The stall output becomes active if SG_RESULT fall below this value.
    driver.SGTHRS(STALL_VALUE);

    Serial.print("\nTesting connection...");
    uint8_t result = driver.test_connection();

    if (result)
    {
        Serial.println("failed!");
        Serial.print("Likely cause: ");

        switch (result)
        {
        case 1:
            Serial.println("loose connection");
            break;
        case 2:
            Serial.println("no power");
            break;
        }

        Serial.println("Fix the problem and reset board.");

        // We need this delay or messages above don't get fully printed out
        delay(100);
        abort();
    }

    Serial.println("OK");

    pinMode(EN_PIN, OUTPUT);
    digitalWrite(EN_PIN, HIGH);
}