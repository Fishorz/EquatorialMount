/*
---------------------------------------------------------------------------------------
----functions list----
1.Manual local polar alignment( Use GPS and gyroscope{陀螺儀} ).
2.input invterval time and exposure time take photo.
3.follow polar alignment rotate.
4.Input rotate speed to rotate.
----User interface-----
1.four button control.
2.Onec TFT display.
----additional functions----
1.There is app to control.
*/
#include <Arduino.h>
#include "TimeLapseControl.hpp"
#include "kickTrigger.hpp"
#include "meun.hpp"
#include "buttom.hpp"
#include "Time.hpp"
#include "motorControl.hpp"
#include "Logger.hpp"
#include "TFTLCD.hpp"
kickTrigger Triggertest;
Logger logger;
Buttom buttom;
Meun meun;
void setup()
{
    Serial.begin(25000);
    delay(100);
    logger.print("startUp");
}

void loop()
{
    int commandHandler = buttom.commandHandler();
    meun.getFunction(commandHandler);
}