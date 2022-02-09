#pragma once
#include "TakePhoto.hpp"

class timeLapseControl
{
private:
    unsigned long time;
    int _istrigger = false;
    int _intervalTime;
    int _exposureTime;
    int _number;
    kickTrigger takePhoto;

public:
    void
    setTimes(int intervalTime, int exposureTime);
    void setNumbers(int number);
};

void timeLapseControl::setTimes(int intervalTime, int exposureTime)
{
    _intervalTime = intervalTime;
    _exposureTime = exposureTime;
}

void timeLapseControl::setNumbers(int number)
{
    _number = number;
}
