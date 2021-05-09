#include <Arduino.h>
#define irLED 5
#define Dir_Pin 6
#define Step_Pin 7
unsigned long mills_INTERVAL_exposureTimeS = 2000;
unsigned long mills_INTERVAL_TimeS = 5000;
unsigned long exposureTime;
unsigned long intervalTime;
bool state = 0; //1 = shutter down ; 0 = shutter up
void print_time(unsigned long time_millis);
void trigger_The_Shutter();
void setup()
{
  Serial.begin(9600);
  Serial.println("start");
}

void trigger_The_Shutter()
{
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(irLED, HIGH);
    delayMicroseconds(11);
    digitalWrite(irLED, LOW);
    delayMicroseconds(11);
  }
  delayMicroseconds(7330);
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(irLED, HIGH);
    delayMicroseconds(11);
    digitalWrite(irLED, LOW);
    delayMicroseconds(11);
  }
}
void stepperMotorControl()
{
}

void loop()
{

  if (millis() >= exposureTime + mills_INTERVAL_exposureTimeS)
  {
    if (state == 0)
    {
      exposureTime += mills_INTERVAL_exposureTimeS;
      //trigger the shutter up
      print_time(exposureTime);
      Serial.println("trigger the shutter up");
      Serial.println("state" + state);
      trigger_The_Shutter();
      state = 1;
    }
  }

  if (millis() >= intervalTime + mills_INTERVAL_TimeS)
  {
    if (state == 1)
    {
      intervalTime += mills_INTERVAL_TimeS;
      //trigger the shutter down
      print_time(intervalTime);
      Serial.println("trigger the shutter down");
      Serial.println("state" + state);
      trigger_The_Shutter();
      state = 0;
    }
  }
}

void print_time(unsigned long time_millis)
{
  Serial.print("Time: ");
  Serial.print(time_millis / 1000);
  Serial.print("s - ");
}