#ifndef GY91sensor_h
#define GY91sensor_h
#include <Arduino.h>
#include <MPU9250_asukiaaa.h>
#include <Adafruit_BMP280.h>
class GY91sensor
{
private:
    Adafruit_BMP280 bmp;
    MPU9250_asukiaaa MPU9250;
    float _aX, _aY, _aZ, _aSqrt, _gX, _gY, _gZ, _mDirection, _mX, _mY, _mZ;
    byte _SDA_PIN, _SCL_PIN;
    void getData();

public:
    void setPin(byte SDA_PIN, byte SCL_PIN);
    int getmDirection();
};

void GY91sensor::setPin(byte SDA_PIN, byte SCL_PIN)
{
    _SDA_PIN = SDA_PIN;
    _SCL_PIN = SCL_PIN;
    Wire.begin();
    MPU9250.setWire(&Wire);

    bmp.begin();
    MPU9250.beginAccel();
    MPU9250.beginGyro();
    MPU9250.beginMag();
}

void GY91sensor::getData()
{

    if (MPU9250.accelUpdate() == 0)
    {
        _aX = MPU9250.accelX();
        _aY = MPU9250.accelY();
        _aZ = MPU9250.accelZ();
        _aSqrt = MPU9250.accelSqrt();
        // Serial.print("accelX: " + String(aX));
        // Serial.print("\taccelY: " + String(aY));
        // Serial.print("\taccelZ: " + String(aZ));
        // Serial.print("\taccelSqrt: " + String(aSqrt));
    }

    if (MPU9250.gyroUpdate() == 0)
    {
        _gX = MPU9250.gyroX();
        _gY = MPU9250.gyroY();
        _gZ = MPU9250.gyroZ();
        // Serial.print("\tgyroX: " + String(gX));
        // Serial.print("\tgyroY: " + String(gY));
        // Serial.print("\tgyroZ: " + String(gZ));
    }

    if (MPU9250.magUpdate() == 0)
    {
        _mX = MPU9250.magX();
        _mY = MPU9250.magY();
        _mZ = MPU9250.magZ();
        _mDirection = MPU9250.magHorizDirection();
        // Serial.print("\tmagX: " + String(mX));
        // Serial.print("\tmaxY: " + String(mY));
        // Serial.print("\tmagZ: " + String(mZ));
        // Serial.print("\thorizontalDirection: " + String(mDirection));
    }
}

int GY91sensor::getmDirection()
{
    getData();
    return (_mDirection);
}
#endif