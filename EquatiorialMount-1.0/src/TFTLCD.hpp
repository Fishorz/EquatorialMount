#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h>

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
    void setup();
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    int _lastMainMeunOrder;
    int _lastSubMeunOrder;
    // bool reflashControl();

public:
    void showTest();
    void showStartMeun();
    void showPerviousTime(int mins, int sec, int oneTenthSec);
    void showMainMeun(int mainMeunOrder);
    void showSubMeun(int subMeunOrder);
    enum mainMenu
    {
        intervalTimeControl_mainMenu,
        exposureTimeControl_mainMenu,
        rotateEnableControl_mainMenu,
        modeSelection_mainMenu,
        autoAimPolarAlignment,
    };
};
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

void TFTLCD::showMainMeun(int mainMeunOrder)
{
    if (_lastMainMeunOrder != mainMeunOrder) // flash onec time when meun order change
    {

        logger.println("mainMeunOrder");
        int selected_X = 30;
        int align_X = 20;
        int textSize = 1;
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextWrap(false);
        tft.setCursor(20, 30);
        tft.setTextSize(textSize);
        tft.setTextColor(ST77XX_YELLOW);

        tft.setCursor(align_X, 30);
        tft.println("Interval Time");
        tft.setCursor(align_X, 50);
        tft.println("Exposure Time");
        tft.setCursor(align_X, 70);
        tft.println("Rotate Speed");
        tft.setCursor(align_X, 90);
        tft.println("Mode Selection");
        tft.setCursor(align_X, 110);
        tft.println("Auto Aim Polar Alignment");

        switch (mainMeunOrder)
        {
        case mainMenu::intervalTimeControl_mainMenu:
            tft.setCursor(selected_X, 30);
            tft.println("Interval Time");
            break;
        case mainMenu::exposureTimeControl_mainMenu:
            tft.setCursor(selected_X, 50);
            tft.println("Exposure Time");
            break;
        case mainMenu::rotateEnableControl_mainMenu:
            tft.setCursor(selected_X, 70);
            tft.println("Rotate Speed");
            break;
        case mainMenu::modeSelection_mainMenu:
            tft.setCursor(selected_X, 90);
            tft.println("Mode Selection");
            break;
        case mainMenu::autoAimPolarAlignment:
            tft.setCursor(selected_X, 110);
            tft.println("Auto Aim Polar Alignment");
            break;
        default:
            break;
        }

        _lastMainMeunOrder = mainMeunOrder;
    }
}

void TFTLCD::showPerviousTime(int mins, int sec, int oneTenthSec)
{
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
