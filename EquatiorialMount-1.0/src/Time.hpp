#pragma once
#include <Arduino.h>
#include "Logger.hpp"

class Time
{
private:
    int _mins;
    int _sec;
    int _oneTenthSec;
    void sec(int sec);
    void mins(int mins);
    void oneTenthSec(int oneTenthSec);

public:
    void minsChange(bool direction);
    void secChange(bool direction);
    void oneTenthSecChange(bool direction);
    int calculateMillisecTime(int mins, int sec, int oneTenthSec);
    int getMillisecTime();
};

void Time::minsChange(bool direction)
{
    (direction) ? _mins++ : _mins--;
}

void Time::secChange(bool direction)
{
    (direction) ? _sec++ : _sec--;
}

void Time::oneTenthSecChange(bool direction)
{
    (direction) ? _oneTenthSec++ : _oneTenthSec--;
}

int Time::getMillisecTime()
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
    // return (_oneTenthSec);
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
    // return (_mins);
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
    // return (_sec);
}