#ifndef TFTLCD_h
#define TFTLCD_h
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h>
#include <Time.hpp>
#include <GY91sensor.hpp>

#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
#define TFT_CS 14
#define TFT_RST 15
#define TFT_DC 32

#elif defined(ESP8266)
#define TFT_CS 4
#define TFT_RST 16
#define TFT_DC 5

#else
// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS 14
#define TFT_RST 15 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 32
#endif

class TFTLCD
{
private:
    // GY91sensor GY91;
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    int _lastMainMeunOrder;
    int _lastSubMeunOrder;
    int _mins;
    int _sec;
    int _oneTenthSec;
    int _magHorizDirection;
    int _angle;
    bool _isDisplayReflash = false; // is the display refash yet?
    // bool reflashControl();

public:
    void setup();
    void displayReflash();
    void showTest();
    void showStartMeun();
    // void showPerviousTime(int mins, int sec, int oneTenthSec);
    void showMainMeun(int mainMeunOrder);
    // void showSubMeun(int subMeunOrder); // abandon

    void showIntervalTimeChange(int timeOrder); // layer 01
    void showExposureTimeChange(int timeOrder); // layer 02
    void showRotateChange(int speed);           // layer 03 speed change
    void showRotateMode(bool mode);             // layer 04 mode select
    void showAutoAPA();                         // layer 05 auto aim polar alignment
    void showTakingTimelapse(Time &intervalTime, Time &exposureTime, int number);

    void getTime(int mins, int sec, int oneTenthSec);
    void getAutoAPAData(int magHorizDirection, int angle);
    enum mainMenu
    {
        intervalTimeControl_mainMenu,
        exposureTimeControl_mainMenu,
        rotateEnableControl_mainMenu,
        modeSelection_mainMenu,
        autoAimPolarAlignment,
    };
};

void TFTLCD::displayReflash()
{
    _isDisplayReflash = false;
}

void TFTLCD::getAutoAPAData(int magHorizDirection, int angle)
{
    _magHorizDirection = magHorizDirection;
    _angle = angle;
}

void TFTLCD::getTime(int mins, int sec, int oneTenthSec)
{
    _mins = mins;
    _sec = sec;
    _oneTenthSec = oneTenthSec;
}

void TFTLCD::setup()
{
    tft.initR(INITR_BLACKTAB); // Init ST7735S chip, black tab
    // GY91.setPin(A4, A5);
}

void TFTLCD::showStartMeun()
{
    setup();
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextWrap(false);
    tft.setCursor(0, 30);
    tft.setTextSize(2);
    tft.println("EQ Mount V0.1");
    delay(50);
}

// void TFTLCD::showSubMeun(int subMeunOrder)
// {
//     if (_isDisplayReflash == false)
//     {
//         _lastSubMeunOrder = subMeunOrder;
//         int connerX = 30;
//         int connerY = 30;
//         logger.print("subMeunOrder");
//         tft.fillScreen(ST77XX_BLACK);
//         tft.setTextWrap(false);
//         tft.setCursor(20, 30);
//         tft.setTextSize(2);
//         tft.setTextColor(ST77XX_YELLOW);
//     }
//     _isDisplayReflash = true;
// }

void TFTLCD::showAutoAPA()
{
    // int selected_Y = 45;
    // int align_Y = 40;
    int textSize = 1;
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextWrap(false);
    tft.setCursor(10, 10);
    tft.setTextSize(textSize);
    tft.setTextColor(ST77XX_YELLOW);
    tft.println("magHorizDirection");
    tft.setCursor(10, 30);
    tft.println(_magHorizDirection);
    logger.print("magHorizDirection = ");
    logger.println(_magHorizDirection);
}

void TFTLCD::showExposureTimeChange(int timeOrder)
{
    if (_isDisplayReflash == false)
    {
        int selected_Y = 45;
        int align_Y = 40;
        int textSize = 1;
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setCursor(10, 10);
        tft.setTextSize(textSize);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Exposure Time");

        switch (timeOrder)
        {
        case 0:
            // display interval time changing mins
            tft.setCursor(20, selected_Y);
            tft.println(_mins);
            tft.setCursor(50, align_Y);
            tft.println(_sec);
            tft.setCursor(80, align_Y);
            tft.println(_oneTenthSec);
            break;

        case 1:
            // display interval time changing sec
            tft.setCursor(20, align_Y);
            tft.println(_mins);
            tft.setCursor(50, selected_Y);
            tft.println(_sec);
            tft.setCursor(80, align_Y);
            tft.println(_oneTenthSec);
            break;

        case 2:
            // display interval time changing oneTenthSec
            tft.setCursor(20, align_Y);
            tft.println(_mins);
            tft.setCursor(50, align_Y);
            tft.println(_sec);
            tft.setCursor(80, selected_Y);
            tft.println(_oneTenthSec);
            break;

        default:
            break;
        }
        _isDisplayReflash = true;
    }
}

void TFTLCD::showIntervalTimeChange(int timeOrder)
{
    int selected_Y = 45;
    int align_Y = 40;
    int textSize = 1;
    if (_isDisplayReflash == false)
    {

        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setCursor(10, 10);
        tft.setTextSize(textSize);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Interval Time");

        switch (timeOrder)
        {
        case 0:
            // display interval time changing mins
            tft.setCursor(20, selected_Y);
            tft.println(_mins);
            tft.setCursor(50, align_Y);
            tft.println(_sec);
            tft.setCursor(80, align_Y);
            tft.println(_oneTenthSec);
            break;

        case 1:
            // display interval time changing sec
            tft.setCursor(20, align_Y);
            tft.println(_mins);
            tft.setCursor(50, selected_Y);
            tft.println(_sec);
            tft.setCursor(80, align_Y);
            tft.println(_oneTenthSec);
            break;

        case 2:
            // display interval time changing oneTenthSec
            tft.setCursor(20, align_Y);
            tft.println(_mins);
            tft.setCursor(50, align_Y);
            tft.println(_sec);
            tft.setCursor(80, selected_Y);
            tft.println(_oneTenthSec);
            break;

        default:
            break;
        }
        _isDisplayReflash = true;
    }
}

void TFTLCD::showRotateChange(int speed)
{
    int textSize = 1;
    if (_isDisplayReflash == false)
    {
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setCursor(10, 10);
        tft.setTextSize(textSize);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Rotate Speed");
        tft.setCursor(30, 30);
        tft.println("Speed = ");
        tft.setCursor(80, 30);
        tft.println(speed);

        _isDisplayReflash = true;
    }
}

void TFTLCD::showRotateMode(bool mode)
{
    int textSize = 1;
    if (_isDisplayReflash == false)
    {
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setCursor(10, 10);
        tft.setTextSize(textSize);
        tft.setTextColor(ST77XX_YELLOW);
        tft.println("Rotate Mode");

        tft.setCursor(30, 30);
        tft.println("Mode V V V ");
        tft.setCursor(40, 50);
        mode ? tft.println("Follow") : tft.println("UnFollw");

        _isDisplayReflash = true;
    }
}

void TFTLCD::showTakingTimelapse(Time &intervalTime, Time &exposureTime, int photoNumber)
{
    Time *_intervalTime = &intervalTime;
    Time *_exposureTime = &exposureTime;
    if (_isDisplayReflash == false)
    {
        int Xspacing = 20;
        // logger.println("Display Taking Timelapse");
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setTextColor(ST77XX_RED);
        tft.setCursor(10, 10);
        tft.setTextSize(1);
        tft.println("Taking Timelapse");
        tft.setCursor(10, 30);
        tft.println("Interval Time");
        tft.setCursor(10 + Xspacing, 45);
        tft.println(_intervalTime->getMins());
        tft.setCursor(10 + 2 * Xspacing, 45);
        tft.println(_intervalTime->getSec());
        tft.setCursor(10 + 3 * Xspacing, 45);
        tft.println(_intervalTime->getOneTenthSec());
        tft.setCursor(10, 60);
        tft.println("Exposure Time");
        tft.setCursor(10 + Xspacing, 75);
        tft.println(_exposureTime->getMins());
        tft.setCursor(10 + 2 * Xspacing, 75);
        tft.println(_exposureTime->getSec());
        tft.setCursor(10 + 3 * Xspacing, 75);
        tft.println(_exposureTime->getOneTenthSec());
        tft.setCursor(10, 90);
        tft.println("Photo:");
        tft.setCursor(45, 90);
        tft.println(photoNumber);

        _isDisplayReflash = true;
    }
}

void TFTLCD::showMainMeun(int mainMeunOrder)
{
    int selected_X = 30;
    int align_X = 20;
    int textSize = 1;
    if (_isDisplayReflash == false)
    {
        logger.println("Show Main Meun.");

        // showStartMeun // Init ST7735S chip, black tab
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setCursor(20, 30);
        tft.setTextSize(textSize);
        tft.setTextColor(ST77XX_YELLOW);
        // logger.println("Setted TFT background");

        switch (mainMeunOrder)
        {
        case mainMenu::intervalTimeControl_mainMenu:
            tft.setCursor(selected_X, 30);
            tft.println("Interval Time");
            tft.setCursor(align_X, 50);
            tft.println("Exposure Time");
            tft.setCursor(align_X, 70);
            tft.println("Rotate Speed");
            tft.setCursor(align_X, 90);
            tft.println("Mode Selection");
            tft.setCursor(align_X, 110);
            tft.println("Auto Aim Polar Alignment");
            logger.println("Select Interval Time.");
            break;
        case mainMenu::exposureTimeControl_mainMenu:
            tft.setCursor(align_X, 30);
            tft.println("Interval Time");
            tft.setCursor(selected_X, 50);
            tft.println("Exposure Time");
            tft.setCursor(align_X, 70);
            tft.println("Rotate Speed");
            tft.setCursor(align_X, 90);
            tft.println("Mode Selection");
            tft.setCursor(align_X, 110);
            tft.println("Auto Aim Polar Alignment");
            logger.println("Select Exposure Time.");
            break;
        case mainMenu::rotateEnableControl_mainMenu:
            tft.setCursor(align_X, 30);
            tft.println("Interval Time");
            tft.setCursor(align_X, 50);
            tft.println("Exposure Time");
            tft.setCursor(selected_X, 70);
            tft.println("Rotate Speed");
            tft.setCursor(align_X, 90);
            tft.println("Mode Selection");
            tft.setCursor(align_X, 110);
            tft.println("Auto Aim Polar Alignment");
            logger.println("Select Rotate Speed.");
            break;
        case mainMenu::modeSelection_mainMenu:
            tft.setCursor(align_X, 30);
            tft.println("Interval Time");
            tft.setCursor(align_X, 50);
            tft.println("Exposure Time");
            tft.setCursor(align_X, 70);
            tft.println("Rotate Speed");
            tft.setCursor(selected_X, 90);
            tft.println("Mode Selection");
            tft.setCursor(align_X, 110);
            tft.println("Auto Aim Polar Alignment");
            logger.println("Select Mode Selection.");
            break;
        case mainMenu::autoAimPolarAlignment:
            tft.setCursor(align_X, 30);
            tft.println("Interval Time");
            tft.setCursor(align_X, 50);
            tft.println("Exposure Time");
            tft.setCursor(align_X, 70);
            tft.println("Rotate Speed");
            tft.setCursor(align_X, 90);
            tft.println("Mode Selection");
            tft.setCursor(selected_X, 110);
            tft.println("Auto Aim Polar Alignment");
            logger.println("Select Auto APA.");
            break;
        default:
            break;
        }
        logger.println("Finish to show main meun.");
    }
    _isDisplayReflash = true;
}

void TFTLCD::showTest()
{
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextWrap(false);
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 30);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(1);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_YELLOW);
    tft.setTextSize(2);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_GREEN);
    tft.setTextSize(3);
    tft.println("Hello World!");
    tft.setTextColor(ST77XX_BLUE);
    tft.setTextSize(4);
    tft.print(1234.567);
}

/*
----functions----
tft.fillScreen(ST77XX_BLACK);
tft.fillRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color1);
tft.drawRect(tft.width() / 2 - x / 2, tft.height() / 2 - x / 2, x, x, color2);
tft.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
tft.fillCircle(x, y, radius, color);
tft.setTextWrap(false);

tft.setCursor(0, 30);
tft.setTextColor(ST77XX_RED);
tft.setTextSize(1);
tft.println("Hello World!");
*/
#endif