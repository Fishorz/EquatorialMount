#ifndef Time_h
#define Time_h
#include <Arduino.h>
#include "Logger.hpp"

class Time
{
private:
    unsigned int _mins;
    unsigned int _sec;
    unsigned int _oneTenthSec;
    void secLimit(unsigned sec);
    void minsLimit(unsigned mins);
    void oneTenthSecLimit(unsigned oneTenthSec);

public:
    void minsChange(bool direction);
    void secChange(bool direction);
    void oneTenthSecChange(bool direction);
    int getMins();
    int getSec();
    int getOneTenthSec();
    int calculateMillisecTime(unsigned mins, unsigned sec, unsigned oneTenthSec);
    unsigned long int getMillisecTime();
};

int Time::getMins()
{
    // this._mins;
    return (this->_mins);
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
    minsLimit(_mins);
}

void Time::secChange(bool direction)
{
    (direction) ? _sec++ : _sec--;
    secLimit(_sec);
}

void Time::oneTenthSecChange(bool direction)
{
    (direction) ? _oneTenthSec++ : _oneTenthSec--;
    this->oneTenthSecLimit(_oneTenthSec);
}

unsigned long int Time::getMillisecTime()
{
    // secLimit(_sec);
    // minsLimit(_mins);
    // oneTenthSecLimit(_oneTenthSec);
    int val = calculateMillisecTime(_mins, _sec, _oneTenthSec);
    return (val);
}

int Time::calculateMillisecTime(unsigned mins, unsigned sec, unsigned oneTenthSec)
{
    int millisec;
    millisec = (_mins * 60 * 1000) + (_sec * 1000) + _oneTenthSec * 100;
    return (millisec);
}

void Time::oneTenthSecLimit(unsigned oneTenthSec)
{
    // if (oneTenthSec > 9)
    // {
    //     oneTenthSec = 9;
    // }
    // else if (oneTenthSec < 0)
    // {
    //     oneTenthSec = 0;
    // }
    // _oneTenthSec = oneTenthSec;
    // return (_oneTenthSec);

    _oneTenthSec = constrain(_oneTenthSec, 0, 9);
}

void Time::minsLimit(unsigned mins)
{
    // if (mins > 59)
    // {
    //     mins = 59;
    // }
    // else if (mins < 0)
    // {
    //     mins = 0;
    // }
    // _mins = mins;
    // return (_mins);
    _mins = constrain(mins, 0, 59);
}

void Time::secLimit(unsigned sec)
{
    // if (sec > 59)
    // {
    //     sec = 59;
    // }
    // else if (sec < 0)
    // {
    //     sec = 0;
    // }
    // _sec = sec;
    // return (_sec);
    _sec = constrain(sec, 0, 59);
}

#endif