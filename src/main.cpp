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

bool LEDstate = LOW;

int lastMeun;
int meun = 0;
bool wasMeunUpdated = false;
bool isTakingPhoto = false;
OneButton button(Button, true);

void updateMeun()
{
  wasMeunUpdated = true;
  lcd.clear();
  // Serial.print("lcd.clear()");
  switch (meun)
  {
  case 0:
  {
    lcd.setCursor(0, 0);
    lcd.print("Interval Time");
    lcd.setCursor(1, 1);
    lcd.print(idleDuration / 1000);
    lcd.print("sec");
    Serial.println("Interval Time");
    // lastIdleDuration = idleDuration;
    break;
  }

  case 1:
  {
    lcd.setCursor(0, 0);
    lcd.print("Exposure Time");
    lcd.setCursor(1, 1);
    lcd.print(exposureDuration / 1000);
    lcd.print("sec");
    Serial.println("Exposure Time");
    // lastExposureDuration = exposureDuration;
    break;
  }

  case 2:
  {
    lcd.setCursor(0, 0);
    lcd.print("Rotate On/Off");
    lcd.setCursor(0, 1);
    if (isRotating == true)
    {
      lcd.print("Rotate On");
    }
    else
    {
      lcd.print("Rotate Stop");
    }
    Serial.println("Rotate On/Off");
    break;
  }

  case 3:
  {
    lcd.setCursor(0, 0);
    lcd.print("taking Photo");
    lcd.setCursor(0, 1);
    lcd.print("photo number");
    lcd.print(photoNumber);
    Serial.println("TakingPhoto");
    break;
  }
  }
}

void takePhotocontrol() //long click
{
  Serial.println("LongPress");
  lcd.clear();
  wasMeunUpdated = false;
  // counter = 0;
  isTakingPhoto = !isTakingPhoto;
  if (isTakingPhoto == true)
  {
    lastMeun = meun;
    Serial.println("TakingPhoto");
    meun = 3;
  }
  else
  {
    meun = lastMeun;
    photoNumber = 0;
    Serial.println("StopTakingPhoto");
  }
  Serial.println("meun=");
  Serial.println(meun);
}

void nextMeun() //one click
{
  Serial.println("Short Press");
  wasMeunUpdated = false;
  // counter = 0;
  if (isTakingPhoto == false)
  {
    Serial.println("Click");
    lcd.clear();
    meun++;
    if (meun > 2)
    {
      meun = 0;
    }
    Serial.println("meun=");
    Serial.println(meun);
  }
  else
  {
    return;
  }
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
  // Serial.println("Shutting");
  // Serial.print("Time=");
  // Serial.println(millis());
  // Serial.println("TakingPhoto");
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
  if (isTakingPhoto == true)
  {
    if (shouldUp || shouldDown)
    {
      // Serial.println(shouldUp ? "UP" : "DOWN");
      if (shouldUp)
      {
        lastUpTime = currentTs;
      }
      if (shouldDown)
      {
        lastDownTime = currentTs;
        photoNumber++;
        wasMeunUpdated = false;
      }
      // Serial.print("Debug2");
      trigger_The_Shutter();
      state = state == 1 ? 0 : 1;
      // Serial.println("");
    }
  }
}

void encoder()
{
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aState != aLastState)
  {
    wasMeunUpdated = false;
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
    wasMeunUpdated = false;
    // Serial.print("clean counterBeark");
    Serial.print("counter: ");
    Serial.println(counter);
  }
  aLastState = aState; // Updates the previous state of the outputA with the current state
  if (counter <= 0)
  {
    counter = 0;
  }
  else if (counter >= 200)
  {
    counter = 200;
  }
}

void timeChange()
{
  switch (meun)
  {
  case 0:
    encoder();
    idleDuration = counter * 1000;
    break;
  case 1:
    encoder();
    exposureDuration = counter * 1000;
    break;
  case 2:
    int lastCounter = 0;
    lastCounter = counter;
    encoder();
    if (lastCounter != counter)
    {
      isRotating = !isRotating;
    }
    break;
  }
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
  button.attachLongPressStart(takePhotocontrol);
  button.attachClick(nextMeun);

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
