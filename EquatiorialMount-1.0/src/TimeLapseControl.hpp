#pragma once
#include "kickTrigger2.h"
#include "Logger.hpp"

class timeLapseControl
{
private:
    unsigned long time;
    int _istrigger = false;
    int _intervalTime = 3000;
    int _exposureTime = 4000;
    int _phtotNumber;
    int _pin;
    kickTrigger2 takePhoto;
    Logger logger;
    bool isTriggerDone = false;
    bool isExposuring = true;
    bool waitingExposure = false;
    bool _buttonState;
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
        // takePhoto.taskSetup();
    }
};

int timeLapseControl::getNumber()
{
    return (_phtotNumber);
}

void timeLapseControl::setTimes(unsigned long int intervalTime, unsigned long int exposureTime)
{
    // _intervalTime = intervalTime;
    // _exposureTime = exposureTime;
}

void timeLapseControl::setNumbers(int number)
{
    _phtotNumber = number;
}

void timeLapseControl::runTimelapse()
{
    // logger.println("running Timelapse");
    unsigned long currentTimes = millis();
    // logger.print("_exposureTime = ");
    // logger.println(_exposureTime);
    // logger.print("_intervalTime = ");
    // logger.println(_intervalTime);
    // logger.print("millis() = ");
    // logger.println(millis());
    // logger.print("isExposuring?");
    // logger.println(isExposuring ? "T" : "F");
    // logger.print("waitingExposure?");
    // waitingExposure ? logger.println("T") : logger.println("F");

    bool pressShutter = (currentTimes - _previousEndTakePhotoTime >= _intervalTime && _buttonState == LOW);
    bool releaseShutter = (currentTimes - _previousStartTakePhotoTime >= _exposureTime && _buttonState == HIGH);

    if (pressShutter || releaseShutter)
    {

        if (pressShutter)
        {
            logger.println("Press the shutter");
            _previousStartTakePhotoTime = currentTimes;
            isTriggerDone = false;
        }

        if (releaseShutter)
        {
            logger.println("release the shutter");
            _previousEndTakePhotoTime = currentTimes;
            _phtotNumber++;
            isTriggerDone = false;
        }
        _buttonState = !_buttonState;
    }

    if (isTriggerDone == false)
    {
        takePhoto.trigger(NULL);
    }
    // if (currentTimes - _previousStartTakePhotoTime > _exposureTime && isExposuring == true) // to control exposure
    // {
    //     if (isTriggerDone == false)
    //     {
    //         isTriggerDone = takePhoto.trigger();
    //         // logger.println("gen signal to ------------------START Exposure");
    //     }
    //     else
    //     {
    //         isExposuring = false;
    //         isTriggerDone = false;
    //         waitingExposure = true; // go to idle a intervalTimes
    //         logger.println("interval count Times");
    //     }
    //     // logger.println("exposure Start");
    // }
    // if (currentTimes - _previousStartTakePhotoTime > _intervalTime && waitingExposure == true) // to control idle status
    // {
    //     if (isTriggerDone == false)
    //     {
    //         isTriggerDone = takePhoto.trigger();
    //         _number++;
    //         _previousStartTakePhotoTime = millis();
    //         // logger.println("gen signal to ---------------------------------------------END Exposure");
    //     }
    //     else
    //     {
    //         waitingExposure = false;
    //         isTriggerDone = false;
    //         isExposuring = true; // next go to exposure
    //         logger.println("exposure count time");
    //     }
    //     // logger.println("exposure End");
    // }
}