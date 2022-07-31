#ifndef kickTrigger_h
#define kickTrigger_h
#pragma once
#include "Logger.hpp"
// for canon 16 pulse delayMicroseconds(7330) 16 pulse
// each pulse is 11us HIGH and 11us LOW

class kickTrigger
{
private:
    unsigned long time;
    int _triggerPin;
    // int _IRledPulse = 11; // us
    int _IRledPulse = 11; // us
    unsigned long _previousTime = 0;
    int _pulseTimes = 16;
    bool _pulseState = LOW;
    int _genHighLowPulseTimes = 0;
    bool isPulseDone;
    int gen16PulseTimes;
    unsigned long lastGenPulseTime;
    bool _istrigger = false;
    bool _istriggedPrinted = false;
    bool gen16Pulse(); // gen the Hgih Low signal and finish the kick motion

public:
    void setPin(int triggerPin);
    bool trigger();
};

void kickTrigger::setPin(int triggerPin)
{
    _triggerPin = triggerPin;
    pinMode(triggerPin, OUTPUT);
    digitalWrite(_triggerPin, LOW);
}

bool kickTrigger::trigger()
{
    unsigned long currentTimes = micros();
    if (isPulseDone && currentTimes - lastGenPulseTime >= 25000)
    {
        lastGenPulseTime = currentTimes;
        isPulseDone = false;
        gen16PulseTimes++;
        // logger.print("pulseTime = ");
        // logger.println(lastGenPulseTime);
        // logger.print("currentTimes = ");
        // logger.println(currentTimes);
    }

    if (gen16PulseTimes == 2)
    {
        gen16PulseTimes = 0;
        return (true);
    }

    if (isPulseDone == false)
    {
        isPulseDone = gen16Pulse();
        // logger.print("isPulseDone?");
        // logger.println(isPulseDone ? "T" : "F");
    }
    return (false);
}

bool kickTrigger::gen16Pulse()
{
    // if trigger doen return ture
    unsigned long currentTimes = micros();
    // unsigned long pulseTimes = _pulseTimes;
    // the signal should like |-|_ (16times)
    if (currentTimes - _previousTime >= _IRledPulse)
    {
        // make the signal to high
        _previousTime = micros();
        if (_pulseState == LOW)
        {
            digitalWrite(_triggerPin, HIGH);
            _pulseState = HIGH;
            _genHighLowPulseTimes++;
            // logger.println("Pinout High");
        }
        else if (_pulseState == HIGH)
        {
            digitalWrite(_triggerPin, LOW);
            _pulseState = LOW;
            // logger.println("Pinout LOW");
        }
        // logger.print("_previousPulseTimes =");
        // logger.println(currentTimes - _previousTime);
    }

    if (_genHighLowPulseTimes >= 16)
    {
        _genHighLowPulseTimes = 0;
        _pulseState = LOW;
        digitalWrite(_triggerPin, LOW);
        // logger.println("finish to gen 16Pulse-----------------------------------------------");
        return (true);
        // if triggered (one time) return ture
        // It mean kick ones time of the button
    }
    else
    {
        return (false);
        // if triggering return false
        // It mean kicking the button and it doesn't finish to kick the button
    }
}
#endif