#pragma once
#include "Logger.hpp"
// #ifndef takePhoto
// #define takePhoto

class kickTrigger
{
private:
    unsigned long time;
    int _triggerPin;
    int _IRledPulse = 11; //ms
    unsigned long _previousTime = 0;
    int _pulseTimes = 16;
    bool _pulseState;
    int _previousPulseTimes = 0;

    bool _istrigger = false;
    bool _istriggedPrinted = false;

public:
    void setPin(int triggerPin);
    bool trigger(); //gen the Hgih Low signal and finish the kick motion
};

void kickTrigger::setPin(int triggerPin)
{
    _triggerPin = triggerPin;
    pinMode(triggerPin, OUTPUT);
}

bool kickTrigger::trigger()
{
    //if trigger doen return ture
    unsigned long currentTimes = micros();
    unsigned long pulseTimes = _pulseTimes;
    // the signal should like |-|_|-|_|-|_|-|_|-|_|-|_|-|_|-|_|-|_|-|_ (16times)
    if (currentTimes - _previousTime >= pulseTimes)
    {
        //make the signal to high
        _previousTime = micros();
        if (_pulseState == LOW)
        {
            digitalWrite(_triggerPin, HIGH);
            _pulseState = HIGH;
            // Serial.println("Pinout High");
        }
        else
        {
            digitalWrite(_triggerPin, LOW);
            _pulseState = LOW;
            // Serial.println("Pinout LOW");
        }
        _previousPulseTimes++;
        // Serial.print("_previousPulseTimes =");
        // Serial.println(_previousPulseTimes);
    }

    if (_previousPulseTimes >= 15)
    {
        _previousPulseTimes = 0;
        return (true);
        //if triggered (one time) return ture
        //It mean kick ones time of the button
    }
    else
    {
        return (false);
        //if triggering return false
        //It mean kicking the button and it doesn't finish to kick the button
    }
}
// #endif