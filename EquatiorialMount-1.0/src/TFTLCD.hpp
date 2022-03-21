#ifndef TFTLCD_h
#define TFTLCD_h
#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h>
#include <Time.hpp>

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
#define TFT_CS 4
#define TFT_RST 6 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 5
#endif

class TFTLCD
{
private:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    int _lastMainMeunOrder;
    int _lastSubMeunOrder;
    int _mins;
    int _sec;
    int _oneTenthSec;
    bool _isDisplayReflash = false; // is the display refash yet?
    // bool reflashControl();

public:
    void setup();
    void displayReflash();
    void showTest();
    void showStartMeun();
    // void showPerviousTime(int mins, int sec, int oneTenthSec);
    void showMainMeun(int mainMeunOrder);
    void showSubMeun(int subMeunOrder); // abandon

    void showIntervalTimeChange(int timeOrder); // layer 01
    void showExposureTimeChange(int timeOrder); // layer 02
    void showRotateChange(int speed);           // layer 03 speed change
    void showRotateMode(bool mode);             // layer 04 mode select
    void autoAPA();                             // layer 05 auto aim polar alignment

    void showTakingTimelapse();
    void getTime(int mins, int sec, int oneTenthSec);
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

void TFTLCD::getTime(int mins, int sec, int oneTenthSec)
{
    _mins = mins;
    _sec = sec;
    _oneTenthSec = oneTenthSec;
}

void TFTLCD::setup()
{
    tft.initR(INITR_BLACKTAB); // Init ST7735S chip, black tab
}

void TFTLCD::showStartMeun()
{
    setup();
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextWrap(false);
    tft.setCursor(0, 30);
    tft.setTextSize(2);
    tft.println("EQ Mount V0.1");
    delay(200);
}

void TFTLCD::showSubMeun(int subMeunOrder)
{
    if (_isDisplayReflash == false)
    {
        _lastSubMeunOrder = subMeunOrder;
        int connerX = 30;
        int connerY = 30;
        logger.print("subMeunOrder");
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setCursor(20, 30);
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_YELLOW);

        switch (subMeunOrder)
        {
        case mainMenu::intervalTimeControl_mainMenu:
            tft.setCursor(connerX, connerY);
            tft.println("Interval Time");
            break;
        case mainMenu::exposureTimeControl_mainMenu:
            tft.setCursor(connerX, connerY);
            tft.println("Exposure Time");
            break;
        case mainMenu::rotateEnableControl_mainMenu:
            tft.setCursor(connerX, connerY);
            tft.println("Rotate Speed");
            break;
        case mainMenu::modeSelection_mainMenu:
            tft.setCursor(connerX, connerY);
            tft.println("Mode Selection");
            break;
        case mainMenu::autoAimPolarAlignment:
            tft.setCursor(connerX, connerY);
            tft.println("Auto Aim Polar Alignment");
            break;
        default:
            break;
        }
    }
    _isDisplayReflash = true;
}

void TFTLCD::showExposureTimeChange(int timeOrder)
{
    if (_isDisplayReflash == false)
    {
        int selected_Y = 50;
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
        case 1:
            // display interval time changing mins
            tft.setCursor(20, selected_Y);
            tft.println(_mins);
            tft.setCursor(50, align_Y);
            tft.println(_sec);
            tft.setCursor(80, align_Y);
            tft.println(_oneTenthSec);
            break;

        case 2:
            // display interval time changing sec
            tft.setCursor(20, align_Y);
            tft.println(_mins);
            tft.setCursor(50, selected_Y);
            tft.println(_sec);
            tft.setCursor(80, align_Y);
            tft.println(_oneTenthSec);
            break;

        case 3:
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
    }
    _isDisplayReflash = true;
}

void TFTLCD::showIntervalTimeChange(int timeOrder)
{
    int selected_Y = 50;
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
        case 1:
            // display interval time changing mins
            tft.setCursor(20, selected_Y);
            tft.println(_mins);
            tft.setCursor(50, align_Y);
            tft.println(_sec);
            tft.setCursor(80, selected_Y);
            tft.println(_oneTenthSec);
            break;

        case 2:
            // display interval time changing sec
            tft.setCursor(20, align_Y);
            tft.println(_mins);
            tft.setCursor(50, selected_Y);
            tft.println(_sec);
            tft.setCursor(80, align_Y);
            tft.println(_oneTenthSec);
            break;

        case 3:
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
    }
    _isDisplayReflash = true;
}

void TFTLCD::showRotateChange(int speed)
{
    if (_isDisplayReflash == false)
    {
    }
}

void TFTLCD::showRotateMode(bool mode)
{
    if (_isDisplayReflash == false)
    {
    }
}

void TFTLCD::showTakingTimelapse()
{
    logger.println("Display Taking Timelapse");
    tft.setCursor(20, 30);
    tft.setTextSize(2);
    tft.println("Display Taking Timelapse");
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

        logger.println("Set TFT background");

        logger.println("Switch func Meun");

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
            logger.println("Display Interval Time.");
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
            logger.println("Display Exposure Time.");
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
            logger.println("Display Rotate Speed.");
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
            logger.println("Display Mode Selection.");
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
            logger.println("Display Auto APA.");
            break;
        default:
            break;
        }
        logger.println("Finish to show main meun.");
    }
    _isDisplayReflash = true;
}

// void TFTLCD::showPerviousTime(int mins, int sec, int oneTenthSec)
// {
// }

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