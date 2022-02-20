#include <Arduino.h>
#include "TimeLapseControl.hpp"
#include "meun.hpp"
kickTrigger Triggertest;

bool istrigger = false;
bool istriggedPrinted = false;
void setup()
{
    Triggertest.setPin(32);
    Serial.begin(25000);
    delay(100);
}

void loop()
{

    if (istrigger == false)
    {
        istrigger = Triggertest.trigger();
        Serial.println("trigging");
    }
    else
    {
        if (istriggedPrinted == false)
        {
            Serial.println("trigged");
            istriggedPrinted = true;
        }
    }
}