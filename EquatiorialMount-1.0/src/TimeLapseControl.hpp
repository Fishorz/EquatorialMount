#pragma once
#include "TakePhoto.hpp"
#include "Logger.hpp"

class timeLapseControl
{
private:
    unsigned long time;
    int _istrigger = false;
    int _intervalTime;
    int _exposureTime;
    int _number;
    kickTrigger takePhoto;
    Logger logger;
    bool isTriggerDone = false;
    bool isExposuring = fasle;
    bool waitingExposure = false;
    unsigned long _previousStartTakePhotoTime;
    unsigned long _previousEndTakePhotoTime;

public:
    void
    setTimes(int intervalTime, int exposureTime);
    void setNumbers(int number);
    void runTimelapse();
    void setpin() { takePhoto.setPin(32); }
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
    insigned long intervalTime = _intervalTime;
    if (currentTimes - _previousStartTakePhotoTime > exposureTime && isExposuring == true) // to control exposure
    {
        if (isTriggerDone == false)
        {
            isTriggerDone = takePhoto.trigger();
            logger.print("gen signal to START Exposure");
        }
        else
        {
            isExposuring = false;
            isTriggerDone = false;
            waitingExposure = ture; //go to idle a intervalTimes
        }
    }
    if (currentTimes - _previousEndTakePhotoTime > intervalTime && waitingExposure == ture) // to control idle status
    {
        if (isTriggerDone == false)
        {
            isTriggerDone = takePhoto.trigger();
            logger.print("gen signal to END Exposure");
        }
        else
        {
            waitingExposure = false;
            isTriggerDone = false;
            isExposuring = ture; //next go to exposure
        }
    }
}