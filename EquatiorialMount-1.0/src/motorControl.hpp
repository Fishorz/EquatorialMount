#include "arduino.h"
#pragma once
class motorControl
{
private:
    bool _direction;
    int _speed;
    bool _mode;
    bool _stepperHigh = true;
    byte _dirPin;
    byte _stepPin;
    const int _speedBelongPolarAilgnment = 1000;
    void rotateFollowPolarAilgnmentWithTakingPhoto();
    void rotateFollowInputWithoutTakingPhoto();
    int _stepIntervalTimeInputSpeed = _speed;
    const int _stepIntervalTimePolarAilgnmentSpeed = _speedBelongPolarAilgnment;
    unsigned long _stepPreviousTime = 0;

public:
    void setSpeed(int speed);
    void setDirection(bool direction);
    void setMode(bool mode);
    void setPin(byte dir, byte step);

    // enum mode
    // {
    //     rotateFollowPolarAilgnmentWithTakingPhoto,
    //     rotateFollowInputWithoutTakingPhoto,
    // };
};

void motorControl::setSpeed(int speed)
{
    _speed = speed;
}

void motorControl::setDirection(bool direction)
{
    _direction = direction;
}

void motorControl::setMode(bool mode)
{
    _mode = mode;
    (mode) ? rotateFollowPolarAilgnmentWithTakingPhoto() : rotateFollowInputWithoutTakingPhoto();
}

void motorControl::setPin(byte dir, byte step)
{
    _dirPin = dir;
    _stepPin = step;
}

void motorControl::rotateFollowPolarAilgnmentWithTakingPhoto()
{
    digitalWrite(_dirPin, _direction);
    unsigned long currentTimes = micros();
    if (currentTimes - _stepIntervalTimePolarAilgnmentSpeed > _stepPreviousTime)
    {
        // stepper motor move half step.
        _stepPreviousTime = micros();
        (_stepperHigh) ? digitalWrite(_stepPin, HIGH) : digitalWrite(_stepPin, LOW);
        _stepperHigh = !_stepperHigh;
    }
}

void motorControl::rotateFollowInputWithoutTakingPhoto()
{
    digitalWrite(_dirPin, _direction);
    unsigned long currentTimes = micros();
    if (currentTimes - _stepIntervalTimeInputSpeed > _stepPreviousTime)
    {
        // stepper motor move half step.
        _stepPreviousTime = micros();
        (_stepperHigh) ? digitalWrite(_stepPin, HIGH) : digitalWrite(_stepPin, LOW);
        _stepperHigh = !_stepperHigh;
    }
}