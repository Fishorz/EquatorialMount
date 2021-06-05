#pragma once
#include <TMCStepper.h>
#include "ConfigFile.h"

TMC2209Stepper driver(SW_RX, SW_TX, R_SENSE, DRIVER_ADDRESS);
void TMCstepperSetup()
{
    driver.beginSerial(115200);
    driver.begin();
    driver.VACTUAL(speed);
    driver.blank_time(24);
    driver.rms_current(RMS); // mA
    driver.microsteps(MICROSTEPS);
    driver.semin(5);
    driver.semax(2);
    driver.sedn(0b01);
    driver.SGTHRS(STALL_VALUE);
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
    digitalWrite(EN_PIN, LOW);
}