#ifndef meun_h
#define meun_h

#include <Arduino.h>
#include "Logger.hpp"
#include "Time.hpp"
#include "TFTLCD.hpp"
#include "motorControl.hpp"
#include "GY91sensor.hpp"

/*meun description------------------------------------------------------------------

--mainMeun--              --sub meun--
interval Time Control     >>01 interval_mins
                          >>02 interval_sec
                          >>03 nterval_millisec
exposure Time Control     >>04 exposure_mins
                          >>05 exposure_sec
                          >>06 exposure_millisec
rotate Control            >>07 speed
mode Selection            >>08 Follow mode                   //control rotation follow polar alignment at taking photo.
                          >>09 Unfollow mode                 //control rotation follow user setup anytime.
auto aim polar alignment  >>10 call auto aim polar function. //自動對準極軸

about the button(s)
0{pervious meun}
1{i-- / meun-}  // time++ or meun++
2{i++ / meun+}  //time-- or meun--
3{select/start-stop take photo}

------------example---------------
selecting main meun interval Time Control, if press 1 to change select
exposure Time Control. if press 3 go to sub meun and select interval_mins

*/
class Meun
{
private:
    byte _lastMeun;
    int _meunState = meunState::atTakingTimelapse;
    // int _meunState = meunState::atMainMeun; // True is at main meun; False is at sub meun
    bool _takingTimelapse = false; // True is taking timelapse, false is in setting.
    byte _mainMeunIndex = 0;
    byte _subMeunIntex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;                     // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus;              // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis(); // auto find polar axis function >> to be class?
    int _mainMeun;
    int _buttonFunction;
    int _speed;

    Time _intervalTimeController;
    Time _exposureTimeController;
    TFTLCD _display;
    GY91sensor GY91;
    motorControl _StepperMotor;
    timeLapseControl timelapse;
    int _lastTimelapseNumber;

    bool timelapseInit = false;

    void mainMeunIndexLimit();
    void subMeunFunctionControl();

    void mainMeunButtonControl();
    void submeunButtonControl();

    void timeSelectingButtonControl();
    int timeSelectChangeIndex = 0;
    void timeChangeButtonControl();
    void exposureTimeButtonControl();
    void exposureTimeChangeButtonControl();
    void takingTimelapseButtonControl();

    void showIntervalTimeChangeWithTime();
    void showExposureTimeChangeWithTime();

    void switchWhichTimeSelectToChange(bool dir);
    void switchWhichTimeSelectedToShow();

    void rotateSpeedButtonControl();
    void rotateModeButtonControl();
    void autoAPAButtonControl();

    int initSetup = false;
    int IndexLimit(int index, int max, int min);

    enum meunState
    {
        atMainMeun,
        atSubMeun, // selecing time, that mean selecting change sec or mins some else.
        atTakingTimelapse,
        changingTime, // selected sec or mins else, and here can increase or decrease sec or which sub meun selected.
        atAutoAPA,
    };

    enum mainMenu
    {
        intervalTimeControl_mainMenu,
        exposureTimeControl_mainMenu,
        rotateSpeedControl_mainMenu,
        modeSelection_mainMenu,
        autoAimPolarAlignment,
    };

    enum intervalTimeControlMeun
    {
        interval_mins,
        interval_sec,
        interval_millisec,
    };

    enum exposureTimeControl
    {
        exposure_mins,
        exposure_sec,
        exposure_millisec,
    };

    enum buttonFunction
    {
        perviousMeun,
        increase,
        decrease,
        select,
        start,
        idle,
    };

public:
    // void mainMeunFunctionControl();

    void meunControlor();
    int getMainMeunOrder();
    int getSubMeunOrder();
    // int getMeunState();
    void getFunction(int getbuttonFunction)
    {
        _buttonFunction = getbuttonFunction;
        meunControlor();
        // if (_atMainMeun == meunState::atSubMeun)
        // {
        //     subMeunFunctionControl();
        // }
    };
};

// void manualControl::getbuttonStatus(){
//     _buttonStatus[0] = digitalRead(_buttonPin[0])};

int Meun::IndexLimit(int index, int max, int min)
{
    index = constrain(index, min, max);
    return (index);
}

void Meun::mainMeunIndexLimit()
{
    if (_mainMeunIndex > 4)
    {
        _mainMeunIndex = 4;
        logger.println("Main meun intex out of range, reset to 4");
        return;
    }
    if (_mainMeunIndex < 0)
    {
        _mainMeunIndex = 0;
        logger.println("Main meun intex out of range, reset to 0");
        return;
    }
}

// int Meun::getMeunState()
// {
//     logger.println("Meun state is ");
//     switch (_atMainMeun)
//     {
//     case meunState::atMainMeun:
//         logger.print("At Main Meun");
//         break;
//     case meunState::atSubMeun:
//         logger.print("At Sub Meun");
//         break;
//     case meunState::atTakingTimelapse:
//         logger.print("Taking Timelapse");
//         break;

//     default:
//         break;
//     }

//     return (_atMainMeun);
// }
void Meun::mainMeunButtonControl()
{
    _display.showMainMeun(_mainMeunIndex);
    if (_buttonFunction == buttonFunction::select)
    {
        _meunState = meunState::atSubMeun; // go to sub meun
        _display.displayReflash();
        // showSubmeunControlor();
        // submeunButtonControl();
        logger.println("From Main Meun to Sub Meun.");
    }

    switch (_buttonFunction)
    {
    case (buttonFunction::increase):
        _mainMeunIndex++;
        _display.displayReflash();
        break;
    case (buttonFunction::decrease):
        _mainMeunIndex--;
        _display.displayReflash();
        break;
    default:
        break;
    }
    mainMeunIndexLimit();
}

void Meun::submeunButtonControl()
{
    if (_buttonFunction == buttonFunction::perviousMeun)
    {
        _subMeunIntex = 0;                  // recovery sub meun intex to 0 order
        _meunState = meunState::atMainMeun; // go to main meun
        logger.println("From Sub Meun go to Main Meun.");
        _display.displayReflash();
        // _display.showMainMeun(_mainMeunIndex);
        // mainMeunButtonControl();
        return;
    }
    // display sub meun which selected
    switch (_mainMeunIndex)
    {
    case mainMenu::intervalTimeControl_mainMenu:
        // logger.println("intervalTimeControl_mainMenu.");
        //_display.showIntervalTimeChange(0);
        timeSelectingButtonControl();
        break;
    case mainMenu::exposureTimeControl_mainMenu:
        // logger.println("exposureTimeControl_mainMenu.");
        timeSelectingButtonControl();
        break;
    case mainMenu::rotateSpeedControl_mainMenu:
        rotateSpeedButtonControl();
        break;
    case mainMenu::modeSelection_mainMenu:
        rotateModeButtonControl();
        break;
    case mainMenu::autoAimPolarAlignment:
        Meun::autoAPAButtonControl();
        break;

    default:
        break;
    }
}

void Meun::autoAPAButtonControl()
{
    if (_buttonFunction == buttonFunction::perviousMeun)
    {
        _display.displayReflash();
        _meunState = meunState::atMainMeun;
    }
    _display.getAutoAPAData(GY91.getmDirection(), 0);
    _display.showAutoAPA();
}

void Meun::rotateModeButtonControl()
{
    // logger.println("rotateModeButtonControl");
    switch (_buttonFunction)
    {
    case buttonFunction::perviousMeun:
        _display.displayReflash();
        _meunState = meunState::atMainMeun;
        break;
    case buttonFunction::increase:
        _rotateDirection = true;
        _display.displayReflash();
        logger.println("change rotate mode");
        break;
    case buttonFunction::decrease:
        _rotateDirection = false;
        _display.displayReflash();
        logger.println("change rotate mode");
        break;
    default:
        break;
    }
    _display.showRotateMode(_rotateDirection);
}

void Meun::rotateSpeedButtonControl()
{
    // logger.println("rotateSpeedButtonControl");
    switch (_buttonFunction)
    {
    case buttonFunction::perviousMeun:
        _meunState = meunState::atMainMeun;
        break;
    case buttonFunction::increase:
        _speed++;
        _display.displayReflash();
        logger.println("increasing rotate speed.");
        break;
    case buttonFunction::decrease:
        _speed--;
        _display.displayReflash();
        logger.println("decreasing rotate speed");
        break;
    default:
        break;
    }
    _display.showRotateChange(_speed);
}

void Meun::timeSelectingButtonControl()
{
    switch (_buttonFunction)
    {
    case buttonFunction::perviousMeun:
        timeSelectChangeIndex = 0;
        _meunState = meunState::atMainMeun;
        break;
    case buttonFunction::increase:
        timeSelectChangeIndex++;
        _display.displayReflash();
        logger.println("TimeChangeIndex++");
        break;
    case buttonFunction::decrease:
        timeSelectChangeIndex--;
        _display.displayReflash();
        logger.println("TimeChangeIndex--");
        break;
    case buttonFunction::select:
        _meunState = meunState::changingTime;
        break;
    default:
        break;
    }

    timeSelectChangeIndex = IndexLimit(timeSelectChangeIndex, 2, 0);
    // Because here is showing the meun, so can not add reflash func here.
    if (_mainMeunIndex == mainMenu::intervalTimeControl_mainMenu)
    {
        showIntervalTimeChangeWithTime();
    }
    else if (_mainMeunIndex == mainMenu::exposureTimeControl_mainMenu)
    {
        showExposureTimeChangeWithTime();
    }
}

void Meun::showIntervalTimeChangeWithTime()
{
    _display.getTime(_intervalTimeController.getMins(), _intervalTimeController.getSec(), _intervalTimeController.getOneTenthSec());
    _display.showIntervalTimeChange(timeSelectChangeIndex);
}

void Meun::showExposureTimeChangeWithTime()
{
    _display.getTime(_exposureTimeController.getMins(), _exposureTimeController.getSec(), _exposureTimeController.getOneTenthSec());
    _display.showExposureTimeChange(timeSelectChangeIndex);
}

void Meun::switchWhichTimeSelectedToShow()
{
    if (_mainMeunIndex == mainMenu::intervalTimeControl_mainMenu)
    {
        _display.displayReflash();
        showIntervalTimeChangeWithTime();
    }
    else if (_mainMeunIndex == mainMenu::exposureTimeControl_mainMenu)
    {
        _display.displayReflash();
        showExposureTimeChangeWithTime();
    }
}

void Meun::switchWhichTimeSelectToChange(bool dir)
{
    switch (timeSelectChangeIndex)
    {
    case 0:
        if (_mainMeunIndex == mainMenu::intervalTimeControl_mainMenu)
        {
            _intervalTimeController.minsChange(dir);
            logger.println("changing int mins time");
        }
        else if (_mainMeunIndex == mainMenu::exposureTimeControl_mainMenu)
        {
            _exposureTimeController.minsChange(dir);
            logger.println("changing exp mins time");
        }
        break;
    case 1:
        if (_mainMeunIndex == mainMenu::intervalTimeControl_mainMenu)
        {
            _intervalTimeController.secChange(dir);
            logger.println("changing int sec time");
        }
        else if (_mainMeunIndex == mainMenu::exposureTimeControl_mainMenu)
        {
            _exposureTimeController.secChange(dir);
            logger.println("changing exp sec time");
        }
        break;
    case 2:
        if (_mainMeunIndex == mainMenu::intervalTimeControl_mainMenu)
        {
            _intervalTimeController.oneTenthSecChange(dir);
            logger.println("changing int OTS time");
        }
        else if (_mainMeunIndex == mainMenu::exposureTimeControl_mainMenu)
        {
            _exposureTimeController.oneTenthSecChange(dir);
            logger.println("changing exp OTS time");
        }
        break;

    default:
        break;
    }
}

void Meun::timeChangeButtonControl() // true is change interval.
{

    switch (_buttonFunction)
    {
    case buttonFunction::perviousMeun:
        _meunState = meunState::atSubMeun;
        break;
    case buttonFunction::increase:
        switchWhichTimeSelectToChange(HIGH);
        switchWhichTimeSelectedToShow();
        break;
    case buttonFunction::decrease:
        switchWhichTimeSelectToChange(LOW);
        switchWhichTimeSelectedToShow();
        break;
    case buttonFunction::select:
        logger.println("Not in use.");
        break;
    default:
        break;
    }
}

void Meun::takingTimelapseButtonControl()
{
    if (timelapseInit == false)
    {
        // timelapse.setNumbers();
        _display.displayReflash();
        timelapse.setTimes(_intervalTimeController.getMillisecTime(), _exposureTimeController.getMillisecTime());
        timelapseInit = true;
    }
    if (_lastTimelapseNumber != timelapse.getNumber())
    {
        _display.displayReflash();
        _lastTimelapseNumber = timelapse.getNumber();
    }
    timelapse.runTimelapse();
    int intervalTimeArray[3] = {_intervalTimeController.getMins(), _intervalTimeController.getSec(), _intervalTimeController.getOneTenthSec()};
    int exposureTimeArray[3] = {_exposureTimeController.getMins(), _exposureTimeController.getSec(), _exposureTimeController.getOneTenthSec()};
    int photoNumber = timelapse.getNumber();

    _display.showTakingTimelapse(intervalTimeArray, exposureTimeArray, photoNumber);
    if (_buttonFunction == buttonFunction::perviousMeun)
    {
        _display.displayReflash();
        timelapseInit = false;
        _meunState = meunState::atMainMeun;
    }
}

void Meun::meunControlor()
{
    if (initSetup == false)
    {
        timelapse.setpin(26);
        GY91.setup();
        _display.setup();
        initSetup = true;
        logger.println("init setup finish.");
    }
    if (_buttonFunction == buttonFunction::start)
    {
        _meunState = meunState::atTakingTimelapse;
        // logger.println("go to time lapse");
    }

    switch (_meunState)
    {
        //-----------------------------------at main meun
    case meunState::atMainMeun:
        mainMeunButtonControl();
        break;
    //-----------------------------------at main meun
    //------------------------------------at sub meun
    case (meunState::atSubMeun):
        submeunButtonControl();
        break;
        //------------------------------------at sub meun

    case (meunState::changingTime):
        // logger.println("changingTime");
        timeChangeButtonControl();
        break;

    case (meunState::atTakingTimelapse):
        takingTimelapseButtonControl();
        break;
    default:
        break;
    }
}

// int Meun::getMainMeunOrder()
// {
//     int MainMeunOrder = _mainMeunIndex;
//     return (MainMeunOrder);
// }

// int Meun::getSubMeunOrder()
// {
//     int subMeunOrder = _subMeunIntex;
//     return (subMeunOrder);
// }

// void meun::mainMeunFunctionControl()
// {

//     // according to main meun index to switch show which meun is selected
//     switch (_mainMeunIntex)
//     {
//     case (1):
//         _mainMeun = mainMenu::intervalTimeControl_mainMenu;
//         // show selecting intervalTimeControl
//         break;
//     case (2):
//         _mainMeun = mainMenu::exposureTimeControl_mainMenu;
//         // show selecting exposureTimeControl
//         break;
//     case (3):
//         _mainMeun = mainMenu::rotateEnableControl_mainMenu;
//         // show selecting rotateEnableControl
//         break;
//     default:
//         break;
//     }
// }

/*
void Meun::subMeunFunctionControl()
{
    // according to sub meun index to switch show which sub meun was selected
    switch (_mainMeun)
    {
    case (mainMenu::intervalTimeControl_mainMenu):
        switch (_subMeunIntex)
        {
        case (intervalTimeControlMeun::interval_mins):
            switch (_buttonFunction)
            {
            case (buttonFunction::increase):
                _display.displayReflash();
                _intervalTimeController.minsChange(HIGH);
                break;
            case (buttonFunction::decrease):
                _display.displayReflash();
                _intervalTimeController.minsChange(LOW);
                break;
            case (buttonFunction::perviousMeun):
                _display.displayReflash();
                _meunState = meunState::atSubMeun;
                break;
            default:
                break;
            }
            _display.getTime(_intervalTimeController.getMins(), _intervalTimeController.getSec(), _intervalTimeController.getOneTenthSec());
            _display.showIntervalTimeChange(1);
            break;
        case (intervalTimeControlMeun::interval_sec):
            switch (_buttonFunction)
            {
            case (buttonFunction::increase):
                // intervalTime.mins(1);
                _intervalTimeController.secChange(HIGH);
                _display.displayReflash();
                break;
            case (buttonFunction::decrease):
                _intervalTimeController.secChange(LOW);
                _display.displayReflash();
                break;
            case (buttonFunction::perviousMeun):
                _display.displayReflash();
                _meunState = meunState::atSubMeun;
                break;
            default:
                break;
            }
            _display.getTime(_intervalTimeController.getMins(), _intervalTimeController.getSec(), _intervalTimeController.getOneTenthSec());
            _display.showIntervalTimeChange(2);
            break;
        case (intervalTimeControlMeun::interval_millisec):
            switch (_buttonFunction)
            {
            case (buttonFunction::increase):
                // intervalTime.mins(1);
                _intervalTimeController.oneTenthSecChange(HIGH);
                _display.displayReflash();
                break;
            case (buttonFunction::decrease):
                _intervalTimeController.oneTenthSecChange(LOW);
                _display.displayReflash();
                break;
            case (buttonFunction::perviousMeun):
                _display.displayReflash();
                _meunState = meunState::atSubMeun;
                break;
            default:
                break;
            }
            _display.getTime(_intervalTimeController.getMins(), _intervalTimeController.getSec(), _intervalTimeController.getOneTenthSec());
            _display.showIntervalTimeChange(3);
            break;

        default:
            break;
        }
        break;

    case (mainMenu::exposureTimeControl_mainMenu):
        switch (_subMeunIntex)
        {
        case (exposureTimeControl::exposure_mins):
            switch (_buttonFunction)
            {
            case (buttonFunction::increase):
                // intervalTime.mins(1);
                _exposureTimeController.minsChange(HIGH);
                break;
            case (buttonFunction::decrease):
                _exposureTimeController.minsChange(LOW);
                break;
            case (buttonFunction::perviousMeun):
                _display.displayReflash();
                _meunState = meunState::atSubMeun;
                break;
            default:
                break;
            }
            _display.displayReflash();
            _display.getTime(_exposureTimeController.getMins(), _exposureTimeController.getSec(), _exposureTimeController.getOneTenthSec());
            _display.showExposureTimeChange(1);
            break;
        case (exposureTimeControl::exposure_sec):
            switch (_buttonFunction)
            {
            case (buttonFunction::increase):
                // intervalTime.mins(1);
                _exposureTimeController.secChange(HIGH);
                break;
            case (buttonFunction::decrease):
                _exposureTimeController.secChange(LOW);
                break;
            case (buttonFunction::perviousMeun):
                _display.displayReflash();
                _meunState = meunState::atSubMeun;
                break;
            default:
                break;
            }
            _display.displayReflash();
            _display.getTime(_exposureTimeController.getMins(), _exposureTimeController.getSec(), _exposureTimeController.getOneTenthSec());
            _display.showExposureTimeChange(2);
            break;
        case (exposureTimeControl::exposure_millisec):
            switch (_buttonFunction)
            {
            case (buttonFunction::increase):
                // intervalTime.mins(1);
                _exposureTimeController.oneTenthSecChange(HIGH);
                break;
            case (buttonFunction::decrease):
                _exposureTimeController.oneTenthSecChange(LOW);
                break;
            case (buttonFunction::perviousMeun):
                _display.displayReflash();
                _meunState = meunState::atSubMeun;
                break;
            default:
                break;
            }
            _display.displayReflash();
            _display.getTime(_exposureTimeController.getMins(), _exposureTimeController.getSec(), _exposureTimeController.getOneTenthSec());
            _display.showExposureTimeChange(3);
            break;
        default:
            break;
        }
        break;

    case (mainMenu::rotateEnableControl_mainMenu):
        switch (_buttonFunction)
        {
        case (buttonFunction::increase):
            _speed++;
            _StepperMotor.setSpeed(_speed);
            break;
        case (buttonFunction::decrease):
            _speed--;
            _StepperMotor.setSpeed(_speed);
            break;
        case (buttonFunction::perviousMeun):
            _display.displayReflash();
            _meunState = meunState::atSubMeun;
            break;
        default:
            break;
        }
        _display.displayReflash();
        _display.showRotateChange(_speed);
        break;

    case (mainMenu::modeSelection_mainMenu):
        switch (_buttonFunction)
        {
        case (buttonFunction::increase):
            _StepperMotor.setMode(true);
            _display.displayReflash();
            _display.showRotateMode(true);
            break;
        case (buttonFunction::decrease):
            _StepperMotor.setMode(false);
            _display.displayReflash();
            _display.showRotateMode(false);
            break;
        case (buttonFunction::perviousMeun):
            _display.displayReflash();
            _meunState = meunState::atSubMeun;
            break;
        default:
            break;
        }
        break;

    case (mainMenu::autoAimPolarAlignment):
        // run auto aim polar alignment
        break;

    default:
        break;
    }
}
*/
#endif
