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
    bool isTriggerDone = false;

    unsigned long _previousStartTakePhotoTime;
    unsigned long _previousEndTakePhotoTime;

public:
    void
    setTimes(int intervalTime, int exposureTime);
    void setNumbers(int number);
    void runTimelapse();
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

void timeLapseControl::runTimelapse()
{
    unsigned long currentTimes = millis();
    unsigned long exposureTime = _exposureTime;
    if (currentTimes - _previousStartTakePhotoTime > exposureTime)
    {
        if (isTriggerDone == false)
        {
            isTriggerDone = takePhoto.trigger();
        }
    }
}