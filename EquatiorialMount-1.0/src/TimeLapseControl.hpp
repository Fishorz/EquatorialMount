#pragma once
#include "kickTrigger.hpp"
#include "Logger.hpp"

class timeLapseControl
{
private:
    unsigned long time;
    int _istrigger = false;
    int _intervalTime;
    int _exposureTime;
    int _number;
    int _pin;
    kickTrigger takePhoto;
    Logger logger;
    bool isTriggerDone = false;
    bool isExposuring = false;
    bool waitingExposure = false;
    unsigned long _previousStartTakePhotoTime = 0;
    unsigned long _previousEndTakePhotoTime = 0;

public:
    void setTimes(unsigned long int intervalTime, unsigned long int exposureTime);
    void setNumbers(int number);
    void runTimelapse();
    int getNumber();
    void setpin(int pin)
    {
        _pin = pin;
        takePhoto.setPin(_pin);
    }
};

int timeLapseControl::getNumber()
{
    return (_number);
}

void timeLapseControl::setTimes(unsigned long int intervalTime, unsigned long int exposureTime)
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
    // logger.println("running Timelapse");
    unsigned long currentTimes = millis();
    logger.print("_exposureTime = ");
    logger.println(_exposureTime);
    logger.print("_intervalTime = ");
    logger.println(_intervalTime);
    logger.print("millis() = ");
    logger.println(millis());
    logger.print("isExposuring?");
    isExposuring ? logger.print("T") : logger.print("F");
    logger.print("waitingExposure?");
    waitingExposure ? logger.print("T") : logger.print("F");
    // unsigned long exposureTime = _exposureTime;
    // unsigned long intervalTime = _intervalTime;
    if (currentTimes - _previousStartTakePhotoTime > _exposureTime && isExposuring == true) // to control exposure
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
            waitingExposure = true; // go to idle a intervalTimes
            logger.print("interval count Times");
        }
        logger.print("exposure Start");
    }
    if (currentTimes - _previousEndTakePhotoTime > _intervalTime && waitingExposure == true) // to control idle status
    {
        if (isTriggerDone == false)
        {
            isTriggerDone = takePhoto.trigger();
            _number++;
            logger.print("gen signal to END Exposure");
        }
        else
        {
            waitingExposure = false;
            isTriggerDone = false;
            isExposuring = true; // next go to exposure
            logger.print("exposure count time");
        }
        logger.print("exposure End");
    }
}