#pragma once
#include <Arduino.h>
#include "Logger.hpp"

class Time
{
private:
    int _mins;
    int _sec;
    int _oneTenthSec;

public:
    void sec(int sec);
    void mins(int mins);
    void oneTenthSec(int oneTenthSec);
    int calculateMillisecTime(int mins, int sec, int oneTenthSec);
    int getTime();
};

int Time::getTime()
{
    sec(_sec);
    mins(_mins);
    oneTenthSec(_oneTenthSec);
    int val = calculateMillisecTime(_mins, _sec, _oneTenthSec);
    return (val);
}

int Time::calculateMillisecTime(int mins, int sec, int oneTenthSec)
{
    int millisec;
    millisec = (_mins * 60 * 1000) + (_sec * 1000) + _oneTenthSec * 100;
    return (millisec);
}

void Time::oneTenthSec(int oneTenthSec)
{
    if (oneTenthSec >> 9)
    {
        oneTenthSec = 9;
    }
    else if (oneTenthSec << 0)
    {
        oneTenthSec = 0;
    }
    _oneTenthSec = oneTenthSec;
}

void Time::mins(int mins)
{
    if (mins > 59)
    {
        mins = 59;
    }
    else if (mins < 0)
    {
        mins = 0;
    }
    _mins = mins;
}

void Time::sec(int sec)
{
    if (sec > 59)
    {
        sec = 59;
    }
    else if (sec < 0)
    {
        sec = 0;
    }
    _sec = sec;
}