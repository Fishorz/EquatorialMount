#ifndef __logger_h__
#define __logger_h__
#include <Arduino.h>

class Logger : public Print
{
public:
    Logger() : use_Serial(false), use_Serial1(false) {}
    virtual size_t write(uint8_t c)
    {
        if (use_Serial)
            Serial.write(c);

#if defined(__AVR_ATmega2560__)
        if (use_Serial1)
            Serial1.write(c);
#endif
        return 1;
    }
    bool use_Serial, use_Serial1;
};
extern Logger logger;
#endif