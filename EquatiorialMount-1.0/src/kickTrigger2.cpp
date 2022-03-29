#include <kickTrigger2.h>


void kickTrigger2::setupTriggerPin(byte pin)
{
    _IRpin = pin;
}

void kickTrigger2::Task1code(void *parameter)
{
    for (int i = 0; i < 16; i++)
    {
        digitalWrite(_IRpin, HIGH);
        delayMicroseconds(11);
        digitalWrite(_IRpin, LOW);
        delayMicroseconds(11);
    }

    delayMicroseconds(7330);

    for (int i = 0; i < 16; i++)
    {
        digitalWrite(_IRpin, HIGH);
        delayMicroseconds(11);
        digitalWrite(_IRpin, LOW);
        delayMicroseconds(11);
    }
}