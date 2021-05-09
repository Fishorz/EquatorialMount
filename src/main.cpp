#include <Arduino.h>
#define irLED 5
#define Dir_Pin 6
#define Step_Pin 7
unsigned long exposureDuration = 2000;
unsigned long idleDuration = 5000;
unsigned long lastUpTime = 0;
unsigned long lastDownTime = 0;

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
  Serial.println("Shutting");
  Serial.print("Time=");
  Serial.println(millis());
}

void stepperMotorControl()
{
}

void loop()
{
  unsigned long currentTs = millis();
  bool shouldUp = (lastUpTime == 0 || currentTs >= lastDownTime + idleDuration) && state == 1;
  bool shouldDown = (lastUpTime != 0 && currentTs >= lastUpTime + exposureDuration) && state == 0;
  Serial.print("Debug1");
  if (shouldUp || shouldDown)
  {
    Serial.print("Debug2");
    trigger_The_Shutter();
    state = state == 1 ? 0 : 1;
  }
}

void print_time(unsigned long time_millis)
{
  Serial.print("Time: ");
  Serial.print(time_millis / 1000);
  Serial.print("s - ");
}