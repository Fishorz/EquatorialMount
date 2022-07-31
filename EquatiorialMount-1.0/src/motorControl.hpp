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
    int _stepIntervalTimeInputSpeed;
    const int _stepIntervalTimePolarAilgnmentSpeed = _speedBelongPolarAilgnment;
    unsigned long _stepPreviousTime = 0;

public:
    void setSpeed(int speed);
    void setDirection(bool direction);
    void setMode(bool mode); // if true rotate Follow Polar Ailgnment With TakingPhoto.
    void setPin(byte dir, byte step);
    void run();

    // enum mode
    // {
    //     rotateFollowPolarAilgnmentWithTakingPhoto,
    //     rotateFollowInputWithoutTakingPhoto,
    // };
};

void motorControl::setSpeed(int speed)
{
    _speed = speed;
    _stepIntervalTimeInputSpeed = _speed * 100;
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
    pinMode(dir, OUTPUT);
    pinMode(step, OUTPUT);
}

void motorControl::rotateFollowPolarAilgnmentWithTakingPhoto()
{
    digitalWrite(_dirPin, HIGH);
    // unsigned long currentTimes = micros();
    // if (currentTimes - _stepIntervalTimePolarAilgnmentSpeed > _stepPreviousTime)
    // {
    //     // stepper motor move half step.
    //     _stepPreviousTime = micros();
    //     (_stepperHigh) ? digitalWrite(_stepPin, HIGH) : digitalWrite(_stepPin, LOW);
    //     _stepperHigh = !_stepperHigh;
    // }
}

void motorControl::rotateFollowInputWithoutTakingPhoto()
{
    digitalWrite(_dirPin, _direction);
    // unsigned long currentTimes = micros();
    // if (currentTimes - _stepIntervalTimeInputSpeed > _stepPreviousTime)
    // {
    //     // stepper motor move half step.
    //     _stepPreviousTime = micros();
    //     (_stepperHigh) ? digitalWrite(_stepPin, HIGH) : digitalWrite(_stepPin, LOW);
    //     _stepperHigh = !_stepperHigh;
    // }
}

void motorControl::run()
{
    unsigned long currentTimes = micros();
    int stepperTime;
    if (_speed == 0 && _mode == false)
    {
        logger.println("speed = 0 and return");
        return;
    }
    stepperTime = (_mode) ? _stepIntervalTimePolarAilgnmentSpeed : _stepIntervalTimeInputSpeed;
    if (currentTimes - stepperTime > _stepPreviousTime)
    {
        // stepper motor move half step.
        _stepPreviousTime = micros();
        (_stepperHigh) ? digitalWrite(_stepPin, HIGH) : digitalWrite(_stepPin, LOW);
        _stepperHigh = !_stepperHigh;
        logger.println("running stepper stepperTime = ");
        logger.print(stepperTime);
    }
}