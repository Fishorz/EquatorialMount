#include <Arduino.h>
#include <OneButton.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "ConfigFile.h"
#include "LCD_setup.h"
#include <TMCStepper.h>
#include "StepperMotorSetup.h"

unsigned long previousTime = 0;
unsigned long stepperDelayTime = 25000; //microseconds

enum Menu
{
  intervalSetup = 0,
  exposureDurationSetup,
  rotateEnable,
  takePhoto,
};

unsigned long lastExposureDuration = 0;
unsigned long exposureDuration = 1; //second
unsigned long lastIdleDuration = 0;
unsigned long idleDuration = 1; //second
unsigned long lastUpTime = 0;
unsigned long lastDownTime = 0;
int rotateSpeed = 1;
int maxRotateSpeed = 20;
int minRotateSpeed = 1;

int photoNumber = 0;

bool isRotating = false;

unsigned long intervalTime;
bool state = 0; //1 = bottom down ; 0 = bottom up
void print_time(unsigned long time_millis);

bool LEDstate = LOW;

int lastMeun = Menu::intervalSetup;
int meun = Menu::intervalSetup;
bool wasMeunUpdated = false; //Is updated the meun yet?
bool isTakingPhoto = false;
OneButton button(Button, true);
Encoder myEnc(outputA, outputB);

void updateMeun()
{
  wasMeunUpdated = true;
  lcd.clear();
  // Serial.print("lcd.clear()");
  switch (meun)
  {
  case Menu::intervalSetup:
  {
    lcd.setCursor(0, 0);
    lcd.print("Interval Time");
    lcd.setCursor(1, 1);
    lcd.print(idleDuration);
    lcd.print("sec");
    // Serial.println("Interval Time");
    lastIdleDuration = idleDuration;
    break;
  }

  case Menu::exposureDurationSetup:
  {
    lcd.setCursor(0, 0);
    lcd.print("Exposure Time");
    lcd.setCursor(1, 1);
    lcd.print(exposureDuration);
    lcd.print("sec");
    // Serial.println("Exposure Time");
    lastExposureDuration = exposureDuration;
    break;
  }

  case Menu::rotateEnable:
  {
    lcd.setCursor(0, 0);
    lcd.print("Rotate On/Off");
    lcd.setCursor(0, 1);
    if (isRotating == true)
    {
      lcd.print("Rotate On");
      // Serial.println("Rotate On");
    }
    else
    {
      lcd.print("Rotate Stop");
      // Serial.println("Rotate Stop");
    }

    break;
  }

  case Menu::takePhoto:
  {
    lcd.setCursor(0, 0);
    lcd.print("taking Photo");
    lcd.setCursor(0, 1);
    lcd.print("I");
    lcd.print(idleDuration);
    lcd.setCursor(5, 1);
    lcd.print("E");
    lcd.print(exposureDuration);
    lcd.setCursor(10, 1);
    lcd.print("N=");
    lcd.print(photoNumber);

    // Serial.println("TakingPhoto");
    break;
  }
  }
}

void takePhotocontrol() //long click
{
  // Serial.println("LongPress");
  lcd.clear();
  wasMeunUpdated = false;
  // counter = 0;
  isTakingPhoto = !isTakingPhoto;
  if (isTakingPhoto == true)
  {
    lastMeun = meun;
    // Serial.println("TakingPhoto");
    meun = Menu::takePhoto;
  }
  else
  {
    meun = lastMeun;
    photoNumber = 0;
    // Serial.println("StopTakingPhoto");
  }
  // Serial.print("meun=");
  // Serial.println(meun);
}

void nextMeun() //one click
{
  // Serial.println("Short Press");
  wasMeunUpdated = false;
  // counter = 0;
  if (isTakingPhoto == false)
  {
    // Serial.println("Click");
    lcd.clear();
    meun++;
    if (meun > 2)
    {
      meun = Menu::intervalSetup;
    }
    // Serial.print("meun=");
    // // Serial.println(meun);

    switch (meun)
    {
    case 0:
      myEnc.write(lastIdleDuration);
      break;
    case 1:
      myEnc.write(lastExposureDuration);
      break;
    default:
      break;
    }

    {
    }
  }
  else
  {
    return;
  }
}

void generateIrLEDSignal()
{
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(irLED, HIGH);
    delayMicroseconds(11);
    digitalWrite(irLED, LOW);
    delayMicroseconds(11);
  }
}

void trigger_The_Shutter()
{
  generateIrLEDSignal();
  delayMicroseconds(7330);
  generateIrLEDSignal();
  // Serial.println("Shutting");
  // Serial.print("Time=");
  // Serial.println(millis());
  // Serial.println("TakingPhoto");
}

void stepperMotorControl()
{
  static uint32_t last_time = 0;
  uint32_t ms = millis();
  driver.VACTUAL(speed);
  
  if (isRotating)
  {
    digitalWrite(EN_PIN, LOW);
  }
  else
  {
    digitalWrite(EN_PIN, HIGH);
  }
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

  bool shouldDown = (currentTs >= lastUpTime + (idleDuration * 1000)) && state == 0;
  bool shouldUp = (currentTs >= lastDownTime + (exposureDuration * 1000)) && state == 1;

  // delay(10);
  // Serial.println(currentTs);
  if (shouldUp || shouldDown)
  {
    // Serial.println(shouldUp ? "UP" : "DOWN");
    if (shouldUp)
    {
      lastUpTime = currentTs;
      photoNumber++;
      wasMeunUpdated = false;
    }
    if (shouldDown)
    {
      lastDownTime = currentTs;
      wasMeunUpdated = false;
    }
    // Serial.print("Debug2");
    trigger_The_Shutter();
    state = state == 1 ? 0 : 1;
    // Serial.println("");
  }
}

void limTime(long num)
{
  if (num > Max_Time_Limit)
  {
    num = Max_Time_Limit;
    myEnc.write(num);
    // Serial.print("Max Num");
  }
  if (num < Min_Time_Limit)
  {
    num = Min_Time_Limit;
    myEnc.write(num);
    // Serial.print("Min Num");
  }
  num = num / 4; //cause the encoder have 4 pulse with 1 click
}

void timeChange()
{
  switch (meun)
  {
  case Menu::intervalSetup:
  {
    unsigned long OldIdleDuration = 0;
    long encoderNum = myEnc.read();

    //limit the max and min Num
    limTime(encoderNum);
    OldIdleDuration = idleDuration;
    idleDuration = encoderNum;
    if (idleDuration != OldIdleDuration)
    {
      wasMeunUpdated = false;
      // Serial.print("idleDuration=");
      // Serial.println(idleDuration);
    }
    break;
  }

  case Menu::exposureDurationSetup:
  {
    unsigned long OldExposureDuration = 0;
    long encoderNum = myEnc.read();

    //limit the max and min Num
    limTime(encoderNum);
    OldExposureDuration = exposureDuration;
    exposureDuration = encoderNum;
    if (exposureDuration != OldExposureDuration)
    {
      wasMeunUpdated = false;
      // Serial.print("exposureDuration=");
      // Serial.println(exposureDuration);
    }
    break;
  }

  case Menu::rotateEnable:
  {
    long rotatingState;
    long oldRotatingState;
    oldRotatingState = rotatingState;
    rotatingState = myEnc.read();
    if (rotatingState != oldRotatingState)
    {
      isRotating = !isRotating;
      myEnc.write(oldRotatingState);
      wasMeunUpdated = false;
    }
    break;
  }

  default:
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(Step_Pin, OUTPUT);
  pinMode(Dir_Pin, OUTPUT);
  pinMode(Button, INPUT_PULLUP);

  // pinMode(PIN_LED, OUTPUT);
  TMCstepperSetup();
  Serial.println("TMCstepperSetup--Done");

  button.attachLongPressStart(takePhotocontrol);
  button.attachClick(nextMeun);

  myEnc.write(10);

  lcd.begin(16, 2);
  lcd.backlight();
  _LcdSetup();
  Serial.println("LcdSetup--Done");
}

void loop()
{
  button.tick();
  if (!wasMeunUpdated)
  {
    updateMeun();
  }

  if (isTakingPhoto)
  {
    Timelapse();
  }
  else
  {
    timeChange();
  }
  stepperMotorControl();
  // Serial.println(digitalRead(Button));
}
