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
    Serial.begin(25000);
    delay(100);
    logger.print("startUp");
    tft.showStartMeun();
    buttom.setButtomPin(A3, A1, A2, A0);
}

void loop()
{
    int command = buttom.commandHandler();
    // Serial.print("command = ");
    // Serial.println(command);
    meun.getFunction(command);

    // switch (meun.getMeunState())
    // {
    // case meunState::atMainMeun:
    //     tft.showMainMeun(meun.getMainMeunOrder());
    //     break;

    // case meunState::atSubMeun:
    //     tft.showSubMeun(meun.getSubMeunOrder());
    //     break;

    // case meunState::atTakingTimelapse:
    //     tft.showTakingTimelapse();
    //     break;
    // default:
    //     break;
    // }
}
