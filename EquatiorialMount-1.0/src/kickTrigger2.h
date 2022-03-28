#include <Arduino.h>

class kickTrigger2
{
private:
    TaskHandle_t Task1;
    byte _IRpin;
    void setup()
    {
        xTaskCreatePinnedToCore(
            trigger, /* Function to implement the task */
            "Task1", /* Name of the task */
            10000,   /* Stack size in words */
            NULL,    /* Task input parameter */
            1,       /* Priority of the task */
            &Task1,  /* Task handle. */
            1);      /* Core where the task should run */
    }
    /* Core where the task should run */

public:
    kickTrigger2();
    void setupTriggerPin(byte pin);
    void trigger(void *parameter);
};
