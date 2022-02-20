#pragma once
#include <Arduino.h>
#include "Logger.hpp"

class meun
{
private:
    byte _buttomNums = 4 - 1;
    byte _lastMeun;
    byte _mainMeunIntex = 0;
    byte _subMeunIntex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;                     // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus;              // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis(); // auto find polar axis function >> to be class?
    int _buttomPin[3];
    bool _buttomStatus[3];
    byte _buttomIndex[3];

    enum mainMenu
    {
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

meun::meun()
{
    getButtomStatus();
    
}