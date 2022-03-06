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
public:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
    void showTest();
    void setup();
};

void TFTLCD::setup()
{
    tft.initR(INITR_BLACKTAB); // Init ST7735S chip, black tab
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