#pragma once
#include <Arduino.h>
#include "Logger.hpp"

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
class meun
{
private:
    byte _lastMeun;
    int _atMainMeun = 0; // 0 is at main meun; 1 is at sub meun
    byte _mainMeunIntex = 0;
    byte _subMeunIntex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;                     // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus;              // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis(); // auto find polar axis function >> to be class?
    int _mainMeun;
    int _buttomFunction;

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
    void mainMeunFunctionControl();
    void subMeunFunctionControl();
    void meunSwitch();
    void getFunction(int getbuttomFunction)
    {
        _buttomFunction = getbuttomFunction;
    };
    meun();
};

// void manualControl::getButtomStatus(){
//     _buttomStatus[0] = digitalRead(_buttomPin[0])};

void meun::meunSwitch()
{
    if (_atMainMeun)
    { // controlling main meun
        if (_buttomFunction == buttomFunction::select)
        {
            _atMainMeun = false;
        }
    }
    else
    { // controllig sub meun
        if (_buttomFunction == buttomFunction::perviousMeun)
        {
            _atMainMeun = ture;
        }
    }
}

void meun::mainMeunFunctionControl()
{
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

    // according to main meun index to switch show which meun is selected
    switch (_mainMeunIntex)
    {
    case (1):
        _mainMeun = mainMenu::intervalTimeControl_mainMenu;
        // show selecting intervalTimeControl
        break;
    case (2):
        _mainMeun = mainMenu::exposureTimeControl_mainMenu;
        // show selecting intervalTimeControl
        break;
    case (3):
        _mainMeun = mainMenu::rotateEnableControl_mainMenu;
        // show selecting intervalTimeControl
        break;
    default:
        break;
    }
}

void meun::subMeunFunctionControl()
{
    // according to sub meun index to switch show which sub meun was selected
    switch (_mainMeun)
    {
    case (mainMenu::intervalTimeControl_mainMenu):
        switch (_subMeunIntex)
        {
        case (intervalTimeControlMeun::interval_mins):
            break;
        case (intervalTimeControlMeun::interval_sec):
            break;
        case (intervalTimeControlMeun::interval_millisec):
            break;
        default:
            break;
        }
        break;
    case (mainMenu::exposureTimeControl_mainMenu):
        switch (_subMeunIntex)
        {
        case (exposureTimeControl::exposure_mins):
            break;
        case (exposureTimeControl::exposure_sec):
            break;
        case (exposureTimeControl::exposure_millisec):
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

meun::meun()
{
}