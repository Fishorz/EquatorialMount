#include <Arduino.h>

class TestCoreTwo
{
private:
    TaskHandle_t Task1;

    /* Core where the task should run */

public:
    void Task1code(void *parameter);

    TestCoreTwo()
    {
        xTaskCreatePinnedToCore(
            Task1code, /* Function to implement the task */
            "Task1",   /* Name of the task */
            10000,     /* Stack size in words */
            NULL,      /* Task input parameter */
            1,         /* Priority of the task */
            &Task1,    /* Task handle. */
            1);        /* Core where the task should run */
    };
};
