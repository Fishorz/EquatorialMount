#pragma once
#include <Arduino.h>
#include "Logger.hpp"

class Time
{
private:
    int _mins;
    int _sec;
    int _oneTenthSec;
    void secLimit(int sec);
    void minsLimit(int mins);
    void oneTenthSecLimit(int oneTenthSec);

public:
    void minsChange(bool direction);
    void secChange(bool direction);
    void oneTenthSecChange(bool direction);
    int getMins();
    int getSec();
    int getOneTenthSec();
    int calculateMillisecTime(int mins, int sec, int oneTenthSec);
    int getMillisecTime();
};

int Time::getMins()
{
    return (_mins);
}
int Time::getSec()
{
    return (_sec);
}
int Time::getOneTenthSec()
{
    return (_oneTenthSec);
}

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
    secLimit(_sec);
    minsLimit(_mins);
    oneTenthSecLimit(_oneTenthSec);
    int val = calculateMillisecTime(_mins, _sec, _oneTenthSec);
    return (val);
}

int Time::calculateMillisecTime(int mins, int sec, int oneTenthSec)
{
    int millisec;
    millisec = (_mins * 60 * 1000) + (_sec * 1000) + _oneTenthSec * 100;
    return (millisec);
}

void Time::oneTenthSecLimit(int oneTenthSec)
{
    if (oneTenthSec > 9)
    {
        oneTenthSec = 9;
    }
    else if (oneTenthSec < 0)
    {
        oneTenthSec = 0;
    }
    _oneTenthSec = oneTenthSec;
    // return (_oneTenthSec);
}

void Time::minsLimit(int mins)
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

void Time::secLimit(int sec)
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