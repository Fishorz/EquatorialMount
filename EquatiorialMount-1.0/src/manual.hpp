#pragma once
#include <Arduino.h>
#include "Logger.hpp"

class manualControl
{
private:
    byte _intex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;        // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus; // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis();
    int _buttomPin[3];
    bool _buttomStatus[3];

public:
    void getButtomStatus();
    void getButtomPin(int buttomPin[]);
};

// void manualControl::getButtomStatus(){
//     _buttomStatus[0] = digitalRead(_buttomPin[0])};

void manualControl::getButtomPin(int buttomPin[])
{
    for (size_t i = 0; i < 3; i++)
    {
        _buttomPin[i] = buttomPin[i];
        logger.println("Set Buttom pin = ");
        logger.print(i);
        logger.print(_buttomPin[i]);
    }
}