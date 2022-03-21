#include <Arduino.h>
#include "Logger.hpp"

class Buttom
{
private:
    int _perviousButtomPin;
    int _numIncreaseButtomPin;
    int _numDecreaseButtomPin;
    int _selectButtomPin;

    bool _perviousButtomStatus;
    bool _lastPerviousButtomStatus;
    bool _numIncreaseButtomStatus;
    bool _lastNumIncreaseButtomStatus;
    bool _numDecreaseButtomStatus;
    bool _lastNumDecreaseButtomStatus;
    bool _selectButtomStatus;
    bool _lastSelectButtomStatus;
    int _command;
    void getButtomStatus();

public:
    void setButtomPin(byte a, byte b, byte c, byte d);
    int commandHandler();

    enum buttomFunction
    {
        perviousMeun,
        increase,
        decrease,
        select,
        start,
        idle,
    };
    // ButtomPin.ButtomPin();
};

void Buttom::setButtomPin(byte a, byte b, byte c, byte d)
{
    _perviousButtomPin = a;
    _numIncreaseButtomPin = b;
    _numDecreaseButtomPin = c;
    _selectButtomPin = d;
}

void Buttom::getButtomStatus()
{
    // logger.println("getButtomStatus");
    _perviousButtomStatus = digitalRead(_perviousButtomPin);
    _numIncreaseButtomStatus = digitalRead(_numIncreaseButtomPin);
    _numDecreaseButtomStatus = digitalRead(_numDecreaseButtomPin);
    _selectButtomStatus = digitalRead(_selectButtomPin);
}

int Buttom::commandHandler()
{
    getButtomStatus();

    // if (_perviousButtomStatus && _selectButtomStatus)
    // {
    //     _command = buttomFunction::start;
    //     return (_command);
    // }

    if (_numIncreaseButtomStatus == LOW && _lastNumIncreaseButtomStatus)
    {
        _command = buttomFunction::increase;
        _lastNumIncreaseButtomStatus = LOW;
        return (_command);
    }

    if (_numDecreaseButtomStatus == LOW && _lastNumDecreaseButtomStatus)
    {
        _command = buttomFunction::decrease;
        _lastNumDecreaseButtomStatus = LOW;
        return (_command);
    }

    if (_perviousButtomStatus == LOW && _lastPerviousButtomStatus)
    {
        _command = buttomFunction::perviousMeun;
        _lastPerviousButtomStatus = LOW;
        return (_command);
    }

    if (_selectButtomStatus == LOW && _lastSelectButtomStatus)
    {
        _command = buttomFunction::select;
        _lastSelectButtomStatus = LOW;
        return (_command);
    }
    _lastSelectButtomStatus = _selectButtomStatus;
    _lastNumIncreaseButtomStatus = _numIncreaseButtomStatus;
    _lastNumDecreaseButtomStatus = _numDecreaseButtomStatus;
    _lastPerviousButtomStatus = _perviousButtomStatus;

    return (buttomFunction::idle);
}
