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
    _oneTenthSec = constrain(_oneTenthSec, 0, 9);
}

void Time::minsLimit(unsigned mins)
{
    _mins = constrain(mins, 0, 59);
}

void Time::secLimit(unsigned sec)
{
    _sec = constrain(sec, 0, 59);
}

#endif