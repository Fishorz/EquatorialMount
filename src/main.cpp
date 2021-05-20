#include <Arduino.h>
#include <OneButton.h>
#include <LiquidCrystal_I2C.h>
#include "pin.h"
#include "LCD_setup.h"

unsigned long lastExposureDuration = 0;
unsigned long exposureDuration = 1000;
unsigned long lastIdleDuration = 0;
unsigned long idleDuration = 500;
unsigned long lastUpTime = 0;
unsigned long lastDownTime = 0;
int rotateSpeed = 1;
int maxRotateSpeed = 20;
int minRotateSpeed = 1;

int photoNumber = 0;

bool isRotating = false;

int counter = 0;
int aState;
int aLastState;
int counterBeark = 0;

unsigned long intervalTime;
bool state = 0; //1 = bottom down ; 0 = bottom up
void print_time(unsigned long time_millis);
void trigger_The_Shutter();

bool LEDstate = LOW;

OneButton button(Button, true);

void doubleClickTest()
{
  Serial.println("x2");
}

void attachLongPressStartTest()
{
  Serial.println("LongPress");
}
void attachClickTest()
{
  Serial.println("Click");
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

void encoder()
{
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState)
  {
    counterBeark++;
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != aState && counterBeark == 1)
    {
      counter++;
      counterBeark++;
    }
    else if (digitalRead(outputB) == aState && counterBeark == 1)
    {
      counter--;
      counterBeark++;
    }

    // Serial.print("counterBeark=");
    // Serial.println(counterBeark);
  }
  if (counterBeark == 3)
  {
    counterBeark = 0;
    // Serial.print("clean counterBeark");
    Serial.print("Position: ");
    Serial.println(counter);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
}

void setup()
{
  Serial.begin(9600);
  pinMode(Step_Pin, OUTPUT);
  pinMode(Dir_Pin, OUTPUT);
  pinMode(outputA, INPUT_PULLUP);
  pinMode(outputB, INPUT_PULLUP);
  pinMode(Button, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  aLastState = digitalRead(outputA);
  Serial.println("start");
  button.attachDoubleClick(doubleClickTest);
  button.attachLongPressStart(attachLongPressStartTest);
  button.attachClick(attachClickTest);

  _LcdSetup();
}

void loop()
{
  // Timelapse();
  button.tick();
  if (wasMeunUpdated == false)
  {
    updateMeun();
  }
  timeChange();
  Timelapse();
  // Serial.println(digitalRead(Button));
}
