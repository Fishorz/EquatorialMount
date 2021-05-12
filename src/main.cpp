#include <Arduino.h>
#define irLED 5
#define Dir_Pin 6
#define Step_Pin 7
unsigned long exposureDuration = 2000;
unsigned long idleDuration = 5000;
unsigned long lastUpTime = 0;
unsigned long lastDownTime = 0;
int rotateSpeed = 1;
int maxRotateSpeed = 20;
int minRotateSpeed = 1;

unsigned long intervalTime;
bool state = 0; //1 = bottom down ; 0 = bottom up
void print_time(unsigned long time_millis);
void trigger_The_Shutter();
void setup()
{
  Serial.begin(9600);
  pinMode(Step_Pin, OUTPUT);
  pinMode(Dir_Pin, OUTPUT);
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
/* -----------------------
1 rev = 360 degrees
Use 1/N steps resolution
1 full step = 1.8 degrees
1 step = 1.8/N degrees
For gear ratio 1:60
1 step(pulse) = (1/N)/60 degrees = 1/(60N) degrees

24hr 360degrees ;
1hr 15 degrees;
1mins = 0.25 degrees;
1s = 0.25/60 degrees;
1/1000s = 0.25/6000 degrees;
1/1000s = (0.25/6000)/( 1/(60N) ) steps = 0.64 steps
1.5625 (1/1000)s = 1 steps
*/
{
  digitalWrite(Step_Pin, HIGH);
  delayMicroseconds(10); //control the speed
  digitalWrite(Step_Pin, LOW);
  delayMicroseconds(10);
}

void print_time(unsigned long time_millis)
{
  Serial.print("Time: ");
  Serial.print(time_millis / 1000);
  Serial.print("s - ");
}

void Timelapse()
{
  unsigned long currentTs = millis();

  bool shouldDown = (currentTs >= lastUpTime + idleDuration) && state == 0;
  bool shouldUp = (currentTs >= lastDownTime + exposureDuration) && state == 1;

  delay(10);
  // Serial.println(currentTs);

  if (shouldUp || shouldDown)
  {
    Serial.println(shouldUp ? "UP" : "DOWN");
    if (shouldUp)
    {
      lastUpTime = currentTs;
    }
    if (shouldDown)
    {
      lastDownTime = currentTs;
    }
    // Serial.print("Debug2");
    trigger_The_Shutter();
    state = state == 1 ? 0 : 1;
    Serial.println("");
  }
}

void loop()
{
  Timelapse();
}