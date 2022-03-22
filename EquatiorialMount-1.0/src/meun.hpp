#ifndef meun_h
#define meun_h

#include <Arduino.h>
#include "Logger.hpp"
#include "Time.hpp"
#include "TFTLCD.hpp"
#include "motorControl.hpp"

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

about the buttom(s)
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
    int _meunState = meunState::atMainMeun; // True is at main meun; False is at sub meun
    bool _takingTimelapse = false;          // True is taking timelapse, false is in setting.
    byte _mainMeunIndex = 0;
    byte _subMeunIntex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;                     // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus;              // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis(); // auto find polar axis function >> to be class?
    int _mainMeun;
    int _buttonFunction;
    Time _intervalTimeController;
    Time _exposureTimeController;
    TFTLCD _display;
    motorControl _StepperMotor;
    int _speed;
    void mainMeunIndexLimit();
    void subMeunFunctionControl();

    void mainMeunButtonControl();
    void submeunButtonControl();

    void intervalTimeButtonControl();
    int _intervalTimeChangeIndex = 0;
    void timeChangeButtonControl();
    void exposureTimeButtonControl();
    int _exposureTimeChangeIndex = 0;
    void exposureTimeChangeButtonControl();

    void rotateSpeedButtonControl();
    void rotateModeButtonControl();
    void autoAPAButtonControl();

    int _LCDinit = false;
    int IndexLimit(int index, int max, int min);

    enum meunState
    {
        atMainMeun,
        atSubMeun, // selecing time, that mean selecting change sec or mins some else.
        atTakingTimelapse,
        changingTime, // selected sec or mins else, and here can increase or decrease sec or which sub meun selected.
    };

    enum mainMenu
    {
        intervalTimeControl_mainMenu,
        exposureTimeControl_mainMenu,
        rotateEnableControl_mainMenu,
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

    enum buttomFunction
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
    void getFunction(int getbuttomFunction)
    {
        _buttonFunction = getbuttomFunction;
        meunControlor();
        // if (_atMainMeun == meunState::atSubMeun)
        // {
        //     subMeunFunctionControl();
        // }
    };
};

// void manualControl::getButtomStatus(){
//     _buttomStatus[0] = digitalRead(_buttomPin[0])};

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
    if (_buttonFunction == buttomFunction::select)
    {
        _meunState = meunState::atSubMeun; // go to sub meun
        _display.displayReflash();
        // showSubmeunControlor();
        // submeunButtonControl();
        logger.println("From Main Meun to Sub Meun.");
    }

    switch (_buttonFunction)
    {
    case (buttomFunction::increase):
        _mainMeunIndex++;
        _display.displayReflash();
        break;
    case (buttomFunction::decrease):
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
    if (_buttonFunction == buttomFunction::perviousMeun)
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
        //_display.showIntervalTimeChange(0);
        intervalTimeButtonControl();
        break;
    case mainMenu::exposureTimeControl_mainMenu:
        _display.showExposureTimeChange(0);
        break;
    case mainMenu::rotateEnableControl_mainMenu:
        logger.println("change rotate speed.");
        break;
    case mainMenu::modeSelection_mainMenu:
        logger.println("change rotate mode.");
        break;

    default:
        break;
    }
}

void Meun::intervalTimeButtonControl()
{
    switch (_buttonFunction)
    {
    case buttomFunction::perviousMeun:
        _meunState = meunState::atMainMeun;
        break;
    case buttomFunction::increase:
        _intervalTimeChangeIndex++;
        _display.displayReflash();
        logger.println("TimeChangeIndex++");
        break;
    case buttomFunction::decrease:
        _intervalTimeChangeIndex--;
        _display.displayReflash();
        logger.println("TimeChangeIndex--");
        break;
    case buttomFunction::select:
        _meunState = meunState::changingTime;
        break;
    default:
        break;
    }
    _intervalTimeChangeIndex = IndexLimit(_intervalTimeChangeIndex, 2, 0);
    _display.showIntervalTimeChange(_intervalTimeChangeIndex);
}

void Meun::timeChangeButtonControl() // true is change interval.
{

    switch (_buttonFunction)
    {
    case buttomFunction::perviousMeun:
        _meunState = meunState::atSubMeun;
        break;
    case buttomFunction::increase:
        if (_mainMeunIndex == mainMenu::intervalTimeControl_mainMenu)
        {
            _intervalTimeController.minsChange(true);
            _display.getTime(_intervalTimeController.getMins(), _intervalTimeController.getSec(), _intervalTimeController.getOneTenthSec());
            _display.displayReflash();
            _display.showIntervalTimeChange(_intervalTimeChangeIndex);
            logger.println("increase int mins time");
            // logger.println(_intervalTimeController.getMins());
            // logger.println(_intervalTimeController.getSec());
        }
        else if (_mainMeunIndex == mainMenu::exposureTimeControl_mainMenu)
        {
            _exposureTimeController.minsChange(true);
            _display.getTime(_exposureTimeController.getMins(), _exposureTimeController.getSec(), _exposureTimeController.getOneTenthSec());
            _display.displayReflash();
            _display.showExposureTimeChange(_exposureTimeChangeIndex);
            logger.println("increase exp mins time");
        }

        break;
    case buttomFunction::decrease:
        if (_mainMeunIndex == mainMenu::intervalTimeControl_mainMenu)
        {
            _intervalTimeController.minsChange(false);
            _display.getTime(_intervalTimeController.getMins(), _intervalTimeController.getSec(), _intervalTimeController.getOneTenthSec());
            _display.displayReflash();
            _display.showIntervalTimeChange(_intervalTimeChangeIndex);
            logger.println("decrease int mins time");
        }
        else if (_mainMeunIndex == mainMenu::exposureTimeControl_mainMenu)
        {
            _exposureTimeController.minsChange(false);
            _display.getTime(_exposureTimeController.getMins(), _exposureTimeController.getSec(), _exposureTimeController.getOneTenthSec());
            _display.displayReflash();
            _display.showExposureTimeChange(_exposureTimeChangeIndex);
            logger.println("decrease exp mins time");
        }
        break;
    case buttomFunction::select:
        logger.println("Not in use.");
        break;
    default:
        break;
    }
}

void Meun::meunControlor()
{
    if (_LCDinit == false)
    {
        _display.setup();
        _LCDinit = true;
        logger.println("init display.");
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
            case (buttomFunction::increase):
                _display.displayReflash();
                _intervalTimeController.minsChange(HIGH);
                break;
            case (buttomFunction::decrease):
                _display.displayReflash();
                _intervalTimeController.minsChange(LOW);
                break;
            case (buttomFunction::perviousMeun):
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
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                _intervalTimeController.secChange(HIGH);
                _display.displayReflash();
                break;
            case (buttomFunction::decrease):
                _intervalTimeController.secChange(LOW);
                _display.displayReflash();
                break;
            case (buttomFunction::perviousMeun):
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
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                _intervalTimeController.oneTenthSecChange(HIGH);
                _display.displayReflash();
                break;
            case (buttomFunction::decrease):
                _intervalTimeController.oneTenthSecChange(LOW);
                _display.displayReflash();
                break;
            case (buttomFunction::perviousMeun):
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
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                _exposureTimeController.minsChange(HIGH);
                break;
            case (buttomFunction::decrease):
                _exposureTimeController.minsChange(LOW);
                break;
            case (buttomFunction::perviousMeun):
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
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                _exposureTimeController.secChange(HIGH);
                break;
            case (buttomFunction::decrease):
                _exposureTimeController.secChange(LOW);
                break;
            case (buttomFunction::perviousMeun):
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
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                _exposureTimeController.oneTenthSecChange(HIGH);
                break;
            case (buttomFunction::decrease):
                _exposureTimeController.oneTenthSecChange(LOW);
                break;
            case (buttomFunction::perviousMeun):
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
        case (buttomFunction::increase):
            _speed++;
            _StepperMotor.setSpeed(_speed);
            break;
        case (buttomFunction::decrease):
            _speed--;
            _StepperMotor.setSpeed(_speed);
            break;
        case (buttomFunction::perviousMeun):
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
        case (buttomFunction::increase):
            _StepperMotor.setMode(true);
            _display.displayReflash();
            _display.showRotateMode(true);
            break;
        case (buttomFunction::decrease):
            _StepperMotor.setMode(false);
            _display.displayReflash();
            _display.showRotateMode(false);
            break;
        case (buttomFunction::perviousMeun):
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
