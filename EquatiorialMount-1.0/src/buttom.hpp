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
    bool _numIncreaseButtomStatus;
    bool _numDecreaseButtomStatus;
    bool _selectButtomStatus;
    int _command;

public:
    void setButtomPin(byte a, byte b, byte c, byte d);
    void getButtomStatus();
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
    if (_perviousButtomStatus && _selectButtomStatus)
    {
        _command = buttomFunction::start;
        return (_command);
    }

    if (_numIncreaseButtomStatus)
    {
        _command = buttomFunction::increase;
        return (_command);
    }

    if (_numDecreaseButtomStatus)
    {
        _command = buttomFunction::decrease;
        return (_command);
    }

    if (_perviousButtomStatus)
    {
        _command = buttomFunction::perviousMeun;
        return (_command);
    }

    if (_selectButtomStatus)
    {
        _command = buttomFunction::select;
        return (_command);
    }
    return(buttomFunction::idle);
}
