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
#include "meun.hpp"
#include "button.hpp"
// #include "Time.hpp"
// #include "motorControl.hpp"
// #include "testClass.hpp"
#include "Logger.hpp"
#include "TFTLCD.hpp"
Logger logger;
button button;
Meun meun;
TFTLCD tft;

enum meunState
{
    atMainMeun,
    atSubMeun, // selecing time, that mean selecting change sec or mins some else.
    atTakingTimelapse,
    changeTimeOrState, // selected sec or mins else, and here can increase or decrease sec or which sub meun selected.
};

enum mainMenu
{
    intervalTimeControl_mainMenu,
    exposureTimeControl_mainMenu,
    rotateEnableControl_mainMenu,
    modeSelection_mainMenu,
    autoAimPolarAlignment,
};

void setup()
{
    Serial.begin(9600);
    delay(100);
    logger.println("startUp");
    tft.showStartMeun();
    // button.setbuttonPin(A3, A1, A2, A0);
    button.setbuttonPin(34, 35, 25, 33);
    // GY91T.setPin(A4, A5);
}

void loop()
{
    int command = button.commandHandler();
    // Serial.print("command = ");
    // Serial.println(command);
    meun.getFunction(command);
}
