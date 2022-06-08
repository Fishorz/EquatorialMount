#include <Arduino.h>
#include "Logger.hpp"

class Button
{
private:
    int _perviousButtonPin;
    int _numIncreaseButtonPin;
    int _numDecreasebuttonPin;
    int _selectbuttonPin;

    bool _perviousButtonStatus;
    bool _lastPerviousbuttonStatus;
    bool _numIncreaseButtonStatus;
    bool _lastNumIncreasebuttonStatus;
    bool _numDecreaseButtonStatus;
    bool _lastNumDecreasebuttonStatus;
    bool _selectbuttonStatus;
    bool _lastSelectbuttonStatus;
    int _command;
    void getButtonStatus();

public:
    void setbuttonPin(byte pervious, byte increase, byte decrease, byte select);
    int commandHandler();

    enum buttonFunction
    {
        perviousMeun,
        increase,
        decrease,
        select,
        start,
        idle,
    };
    // buttonPin.buttonPin();
};

void Button::setbuttonPin(byte pervious, byte decrease, byte increase, byte select)
{
    _perviousButtonPin = pervious;
    _numIncreaseButtonPin = increase;
    _numDecreasebuttonPin = decrease;
    _selectbuttonPin = select;

    pinMode(_perviousButtonPin, INPUT);
    pinMode(_numIncreaseButtonPin, INPUT);
    pinMode(_numDecreasebuttonPin, INPUT);
    pinMode(_selectbuttonPin, INPUT);
}

void Button::getButtonStatus()
{
    // logger.println("getbuttonStatus");
    _perviousButtonStatus = digitalRead(_perviousButtonPin);
    _numIncreaseButtonStatus = digitalRead(_numIncreaseButtonPin);
    _numDecreaseButtonStatus = digitalRead(_numDecreasebuttonPin);
    _selectbuttonStatus = digitalRead(_selectbuttonPin);
}

int Button::commandHandler()
{
    getButtonStatus();

    if (_perviousButtonStatus && _selectbuttonStatus)
    {
        _command = buttonFunction::start;
        // logger.println("Button start");
        return (_command);
    }

    if (_numIncreaseButtonStatus == LOW && _lastNumIncreasebuttonStatus)
    {
        _command = buttonFunction::increase;
        _lastNumIncreasebuttonStatus = LOW;
        // logger.println("Button increase");
        return (_command);
    }

    if (_numDecreaseButtonStatus == LOW && _lastNumDecreasebuttonStatus)
    {
        _command = buttonFunction::decrease;
        _lastNumDecreasebuttonStatus = LOW;
        // logger.println("Button decrease");
        return (_command);
    }

    if (_perviousButtonStatus == LOW && _lastPerviousbuttonStatus)
    {
        _command = buttonFunction::perviousMeun;
        _lastPerviousbuttonStatus = LOW;
        // logger.println("Button perviousMeun");
        return (_command);
    }

    if (_selectbuttonStatus == LOW && _lastSelectbuttonStatus)
    {
        _command = buttonFunction::select;
        _lastSelectbuttonStatus = LOW;
        // logger.println("Button select");
        return (_command);
    }
    _lastSelectbuttonStatus = _selectbuttonStatus;
    _lastNumIncreasebuttonStatus = _numIncreaseButtonStatus;
    _lastNumDecreasebuttonStatus = _numDecreaseButtonStatus;
    _lastPerviousbuttonStatus = _perviousButtonStatus;

    return (buttonFunction::idle);
}
