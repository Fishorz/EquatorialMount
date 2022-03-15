#pragma once
#include <Arduino.h>
#include "Logger.hpp"
#include "Time.hpp"

/*meun description------------------------------------------------------------------

--mainMeun--              --sub meun--
interval Time Control     >>interval_mins
                          >>interval_sec
                          >>interval_millisec
exposure Time Control     >>exposure_mins
                          >>exposure_sec
                          >>exposure_millisec
rotate Control            >>speed
mode Selection            >>Follow mode                   //control rotation follow polar alignment at taking photo.
                          >>Unfollow mode                 //control rotation follow user setup anytime.
auto aim polar alignment  >>call auto aim polar function. //自動對準極軸

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
    int _atMainMeun = meunState::atMainMeun; // True is at main meun; False is at sub meun
    bool _takingTimelapse = false;           // True is taking timelapse, false is in setting.
    byte _mainMeunIntex = 0;
    byte _subMeunIntex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;                     // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus;              // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis(); // auto find polar axis function >> to be class?
    int _mainMeun;
    int _buttomFunction;
    Time intervalTime;
    Time exposureTime;
    void _indexLimit();

    enum meunState
    {
        atMainMeun,
        atSubMeun, // selecing time, that mean selecting change sec or mins some else.
        atTakingTimelapse,
        changeTimeOrState, // selected sec or mins else, and here can increase or decrease sec or which sub meun selected.
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
    void subMeunFunctionControl();
    void meunControlor();
    int getMainMeunOrder();
    int getSubMeunOrder();
    int getMeunState();
    void getFunction(int getbuttomFunction)
    {
        _buttomFunction = getbuttomFunction;
        meunControlor();
        // if (_atMainMeun == meunState::atSubMeun)
        // {
        //     subMeunFunctionControl();
        // }
    };
};

// void manualControl::getButtomStatus(){
//     _buttomStatus[0] = digitalRead(_buttomPin[0])};

void Meun::_indexLimit()
{
    if (_mainMeunIntex > 4)
    {
        _mainMeunIntex = 4;
    }
    if (_mainMeunIntex < 0)
    {
        _mainMeunIntex = 0;
    }
    logger.print("_mainMeunIntex =");
    logger.println(_mainMeunIntex);
}

int Meun::getMeunState()
{
    logger.print("_atMainMeun?");
    switch (_atMainMeun)
    {
    case meunState::atMainMeun:
        logger.println("At Main Meun");
        break;
    case meunState::atSubMeun:
        logger.println("At Sub Meun");
        break;
    case meunState::atTakingTimelapse:
        logger.println("Taking Timelapse");
        break;

    default:
        break;
    }

    return (_atMainMeun);
}

void Meun::meunControlor()
{
    switch (_atMainMeun)
    {
        //-----------------------------------at main meun
    case meunState::atMainMeun:
        if (_buttomFunction == buttomFunction::select)
        {
            _atMainMeun = meunState::atSubMeun; // go to sub meun
        }

        switch (_buttomFunction)
        {
        case (buttomFunction::increase):
            _mainMeunIntex++;
            break;
        case (buttomFunction::decrease):
            _mainMeunIntex--;
            break;
        default:
            break;
        }
        break;
    //-----------------------------------at main meun
    //------------------------------------at sub meun
    case (meunState::atSubMeun):
        if (_buttomFunction == buttomFunction::perviousMeun)
        {
            _subMeunIntex = 0;                   // recovery sub meun intex to 0 order
            _atMainMeun = meunState::atMainMeun; // go to main meun
        }
        else if (_buttomFunction == buttomFunction::select)
        {
            _atMainMeun = meunState::changeTimeOrState;
        }

        switch (_buttomFunction)
        {
        case (buttomFunction::increase):
            _subMeunIntex++;
            break;
        case (buttomFunction::decrease):
            _subMeunIntex--;
            break;
        default:
            break;
        }
        //------------------------------------at sub meun
        //------------------------------------at changeTimeOrState
    case (meunState::changeTimeOrState):
        subMeunFunctionControl();
        break;
        //------------------------------------at changeTimeOrState
    default:
        break;
    }
    _indexLimit();
}

int Meun::getMainMeunOrder()
{
    int MainMeunOrder = _mainMeunIntex;
    return (MainMeunOrder);
}

int Meun::getSubMeunOrder()
{
    int subMeunOrder = _subMeunIntex;
    return (subMeunOrder);
}

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

void Meun::subMeunFunctionControl()
{
    // according to sub meun index to switch show which sub meun was selected
    switch (_mainMeun)
    {
    case (mainMenu::intervalTimeControl_mainMenu):
        switch (_subMeunIntex)
        {
        case (intervalTimeControlMeun::interval_mins):
            switch (_buttomFunction)
            {
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                intervalTime.minsChange(HIGH);
                break;
            case (buttomFunction::decrease):
                intervalTime.minsChange(LOW);
                break;
            default:
                break;
            }
            break;
        case (intervalTimeControlMeun::interval_sec):
            switch (_buttomFunction)
            {
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                intervalTime.secChange(HIGH);
                break;
            case (buttomFunction::decrease):
                intervalTime.secChange(LOW);
                break;
            default:
                break;
            }
            break;
        case (intervalTimeControlMeun::interval_millisec):
            switch (_buttomFunction)
            {
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                intervalTime.oneTenthSecChange(HIGH);
                break;
            case (buttomFunction::decrease):
                intervalTime.oneTenthSecChange(LOW);
                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
        break;
    case (mainMenu::exposureTimeControl_mainMenu):
        switch (_subMeunIntex)
        {
        case (exposureTimeControl::exposure_mins):
            switch (_buttomFunction)
            {
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                exposureTime.minsChange(HIGH);
                break;
            case (buttomFunction::decrease):
                exposureTime.minsChange(LOW);
                break;
            default:
                break;
            }
            break;
        case (exposureTimeControl::exposure_sec):
            switch (_buttomFunction)
            {
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                exposureTime.secChange(HIGH);
                break;
            case (buttomFunction::decrease):
                exposureTime.secChange(LOW);
                break;
            default:
                break;
            }
            break;
        case (exposureTimeControl::exposure_millisec):
            switch (_buttomFunction)
            {
            case (buttomFunction::increase):
                // intervalTime.mins(1);
                exposureTime.oneTenthSecChange(HIGH);
                break;
            case (buttomFunction::decrease):
                exposureTime.oneTenthSecChange(LOW);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        break;
    case (mainMenu::rotateEnableControl_mainMenu):
        break;
    default:
        break;
    }
}

// Meun::Meun()
// {
//     getFunction();
// }
