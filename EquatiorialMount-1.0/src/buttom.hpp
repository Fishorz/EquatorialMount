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

public:
    void getButtomPin(byte a, byte b, byte c, byte d);
    void getButtomStatus();
    char command();
    // ButtomPin.ButtomPin();
};

void Buttom::getButtomPin(byte a, byte b, byte c, byte d)
{
    _perviousButtomPin = a;
    _numIncreaseButtomPin = b;
    _numDecreaseButtomPin = c;
    _selectButtomPin = d;
}

void Buttom::getButtomStatus()
{
    _perviousButtomStatus = digitalRead(_perviousButtomPin);
    _numIncreaseButtomStatus = digitalRead(_numIncreaseButtomPin);
    _numDecreaseButtomStatus = digitalRead(_numDecreaseButtomPin);
    _selectButtomStatus = digitalRead(_selectButtomPin);
}

char Buttom::command()
{
    if (_perviousButtomStatus && _selectButtomStatus)
    {
        char start = 'START_';
        return (start);
    }
}