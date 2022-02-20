#pragma once
#include <Arduino.h>
#include "Logger.hpp"

class meun
{
private:
    byte _buttomNums = 4 - 1;
    byte _lastMeun;
    int _meunState = 0; // 0 is at main meun; 1 is at sub meun
    byte _mainMeunIntex = 0;
    byte _subMeunIntex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;                     // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus;              // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis(); // auto find polar axis function >> to be class?
    int _buttomPin[4];
    bool _buttomStatus[4];
    byte _buttomIndex[4];
    int _mainMeun;

    enum mainMenu
    {
        mainMeun,
        intervalTimeControl,
        exposureTimeControl,
        rotateEnableControl,
        takePhoto,
        modeSelection,
    };

    // about the buttom(s) 0{back to main meun/pervious} 1{i-- / meun-} 2{i++ / meun+} 3{select/start-stop take photo}

public:
    void getButtomStatus();
    void getButtomPin(int buttomPin[]);
    void mainMeunButtomControl();
    meun();
};

// void manualControl::getButtomStatus(){
//     _buttomStatus[0] = digitalRead(_buttomPin[0])};

void meun::getButtomPin(int buttomPin[])
{
    for (size_t i = 0; i < _buttomNums; i++)
    {
        _buttomPin[i] = buttomPin[i];
        logger.println("Set Buttom pin = ");
        logger.print(i);
        logger.print(_buttomPin[i]);
    }
}

void meun::getButtomStatus()
{
    for (size_t i = 0; i < _buttomNums; i++)
    {
        _buttomStatus[i] = digitalRead(_buttomPin[i]);
    }
}

void meun::mainMeunButtomControl()
{
    getButtomStatus();
    switch (_meunState)
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }
    if (_buttomStatus[0])
    {
        // 0{back to main meun/pervious}
        logger.println("back to main meun");
    }

    if (_buttomStatus[1])
    {
        // 1{i-- / meun-}
        logger.println("i-- / meun-");
    }

    if (_buttomStatus[2])
    {
        // 2{i++ / meun+}
        logger.println("i++ / meun+");
    }

    if (_buttomStatus[3])
    {
        // 3{select/start-stop take photo}
        logger.println("select/start-stop take photo");
    }
}

meun::meun()
{
}