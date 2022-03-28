#include <Arduino.h>

class kickTrigger2
{
private:
    TaskHandle_t Task1;
    byte _IRpin;
    /* Core where the task should run */

public:
    void taskSetup();
    void setupTriggerPin(byte pin);
    void trigger(void *parameter);
};

void kickTrigger2::taskSetup()
{
    xTaskCreatePinnedToCore(
        trigger, /* Function to implement the task */
        "Task2", /* Name of the task */
        10000,   /* Stack size in words */
        NULL,    /* Task input parameter */
        1,       /* Priority of the task */
        &Task1,  /* Task handle. */
        1);      /* Core where the task should run */
    delay(500);
}

void kickTrigger2::setupTriggerPin(byte pin)
{
    _IRpin = pin;
}

void kickTrigger2::trigger(void *parameter)
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