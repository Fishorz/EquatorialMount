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
    byte _buttomNums = 4 - 1;
    byte _lastMeun;
    int _meunState = 0; // 0 is at main meun; 1 is at sub meun
    byte _mainMeunIntex = 0;
    byte _subMeunIntex = 0;
    int _intervalTime;
    int _exposureTime;
    bool _rotateDirection;                     // ture is clockwise, false is anti-clockwise
    bool _rotateAtPhotoingStatus;              // ture is take photos with rotate, false is take photo without rotate
    void _semiAutomaticAlignmentOfpolarAxis(); // auto find polar axis function >> to be class?
    int _buttomPin[4];
    bool _buttomStatus[4];
    byte _buttomIndex[4];
    int _mainMeun;

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

public:
    void getButtomStatus();
    void getButtomPin(int buttomPin[]);
    void mainMeunButtomControl();
    void subMeunButtomControl();
    meun();
};

// void manualControl::getButtomStatus(){
//     _buttomStatus[0] = digitalRead(_buttomPin[0])};

void meun::getButtomPin(int buttomPin[])
{
    for (size_t i = 0; i < _buttomNums; i++)
    {
        _buttomPin[i] = buttomPin[i];
        logger.println("Set Buttom pin = ");
        logger.print(i);
        logger.print(_buttomPin[i]);
    }
}

void meun::getButtomStatus()
{
    for (size_t i = 0; i < _buttomNums; i++)
    {
        _buttomStatus[i] = digitalRead(_buttomPin[i]);
    }

    if (_buttomStatus[0])
    {
        // 0{back to main meun/pervious}
        logger.println("back to main meun");
    }

    if (_buttomStatus[1])
    {
        // 1{i-- / meun-}
        logger.println("i-- / meun-");
    }

    if (_buttomStatus[2])
    {
        // 2{i++ / meun+}
        logger.println("i++ / meun+");
    }

    if (_buttomStatus[3])
    {
        // 3{select/start-stop take photo}
        logger.println("select/start-stop take photo");
    }
}

void meun::mainMeunButtomControl()
{
    getButtomStatus();

    if (_buttomStatus[2])
    {
        _mainMeunIntex++;
    }
    else if (_buttomStatus[1])
    {
        _mainMeunIntex--;
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

void meun::subMeunButtomControl()
{
    getButtomStatus();
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