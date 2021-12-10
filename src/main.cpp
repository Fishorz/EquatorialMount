#include <Arduino.h>
#include <OneButton.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "ConfigFile.h"
#include "LCD_setup.h"
#include <TMCStepper.h>
#include "StepperMotorSetup.h"

unsigned long updateMeunTime = 0; //update meun each second
unsigned long previousTime = 0;
unsigned int stepperDelayTime = 28125; //microseconds

bool triggerTheShutterFirstTimeState = false;
bool triggerTheShutterSecondTimeState = false;
bool triggerTheShutterDone = false;
unsigned long triggerTheShutterPreviousTime = millis();
unsigned int triggerTheShutterintervalTime = 7330; //(ms)

bool RotateMode = true; //Ture is always rotate; false is rotate when idling

enum Menu
{
  intervalSetup = 0,
  exposureDurationSetup,
  rotateEnable,
  takePhoto,
  modeSelection,
};

//LCD display parameter
unsigned long takingPhotoBacklightControlTimes = 0;
unsigned long displayIdlingControlTimes = 0;
bool isBacklightOn = true;
bool isDisplayIdling = false;

unsigned long lastMeunExposureDuration = 0;
unsigned long exposureDuration = 10; //second
unsigned long lastMeunIdleDuration = 0;
unsigned long idleDuration = 10; //second
unsigned long lastTakePhotoTime = 0;
unsigned long lastIdleTime = 0;

unsigned long currentTimesMillis = 0;
unsigned long currentTimeMicros = 0;

long idleDurationPrint = 0;
long exposureDurationPrint = 0;
int rotateSpeed = 1;
int maxRotateSpeed = 20;
int minRotateSpeed = 1;

int photoNumber = 0;

bool isRotating = false;

bool state = 0; //1 = bottom down ; 0 = bottom up
void print_time(unsigned long time_millis);

bool LEDstate = LOW;

int lastMeun = Menu::intervalSetup;
int meun = Menu::intervalSetup;
bool isStartTimelapse = false;

unsigned long startTimelapseTime = 0; //to count the display idle or exposuring time.
bool takingPhoto = true;              // True is idlling, False is exposuring.

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

  case Menu::modeSelection:
  {
    lcd.setCursor(0, 0);
    lcd.print("Mode");
    lcd.setCursor(1, 1);
    if (RotateMode == true)
    {
      lcd.print("Always Rotate");
      //always rotate
    }
    else
    {
      lcd.print("Rotate in Idle");
    }

    break;
  }

  case Menu::intervalSetup:
  {
    lcd.setCursor(0, 0);
    lcd.print("Interval Time");
    lcd.setCursor(1, 1);
    lcd.print(idleDuration);
    lcd.print("sec");
    // Serial.println("Interval Time");
    lastMeunIdleDuration = idleDuration;
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
    lastMeunExposureDuration = exposureDuration;
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
    if (isRotating)
    {
      lcd.print("Taking Photo(R)");
    }
    else
    {
      lcd.print("taking Photo");
    }
    lcd.setCursor(0, 1);
    lcd.print("I");
    // lcd.print(idleDuration);

    lcd.print(idleDurationPrint);

    lcd.setCursor(5, 1);
    lcd.print("E");
    lcd.print(exposureDurationPrint);
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
    // startTimelapseTime = millis();
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
      myEnc.write(lastMeunIdleDuration);
      break;
    case 1:
      myEnc.write(lastMeunExposureDuration);
      break;
    default:
      break;
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
  unsigned long currentTs = micros();
  if (triggerTheShutterFirstTimeState == false)
  {
    triggerTheShutterPreviousTime = currentTs;
    generateIrLEDSignal();
    triggerTheShutterFirstTimeState = true;
  }
  if (currentTs - triggerTheShutterPreviousTime >= triggerTheShutterintervalTime)
  {
    generateIrLEDSignal();
    triggerTheShutterSecondTimeState = true;
  }

  if (triggerTheShutterFirstTimeState && triggerTheShutterSecondTimeState == true)
  {
    triggerTheShutterFirstTimeState = false;
    triggerTheShutterSecondTimeState = false;
    triggerTheShutterDone = true;
  }
  // Serial.println("Shutting");
  // Serial.print("Time=");
  // Serial.println(millis());
  // Serial.println("TakingPhoto");
}

void stepperMotorControl()
{
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
  // driver.VACTUAL(speed);

  // if (isRotating)
  // {
  //   digitalWrite(EN_PIN, LOW);
  // }
  // else
  // {
  //   digitalWrite(EN_PIN, HIGH);
  // }
  unsigned long currentTs = micros();
  if (isRotating)
  {
    // Serial.print("rotating");
    digitalWrite(EN_PIN, LOW);
    // unsigned long currenTimeMicros = micros();
    if (currentTs - previousTime >= stepperDelayTime)
    {
      digitalWrite(Step_Pin, HIGH);
      previousTime = currentTs;
    }
    //delay stepperDelayTime
    if (currentTs - (previousTime + stepperDelayTime) >= stepperDelayTime)
    {
      digitalWrite(Step_Pin, LOW);
    }
  }
  else
  {
    digitalWrite(EN_PIN, LOW);
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

  bool idle = (currentTs - lastTakePhotoTime >= (idleDuration * 1000)) && state == 1;
  bool takephoto = (currentTs - lastIdleTime >= (exposureDuration * 1000)) && state == 0;

  // delay(10);
  // Serial.println(currentTs);
  if (takephoto || idle)
  {
    startTimelapseTime = currentTs;
    // Serial.println(shouldUp ? "UP" : "DOWN");
    if (takephoto)
    {
      lastTakePhotoTime = currentTs;
      photoNumber++;
      takingPhoto = true;
      // Serial.println("takephoto = true");
      // wasMeunUpdated = false;
    }
    if (idle)
    {
      lastIdleTime = currentTs;
      takingPhoto = false;
      // Serial.println("idle = true");
      // wasMeunUpdated = false;
    }
    // Serial.print("Debug2");

    trigger_The_Shutter();
    state = state == 1 ? 0 : 1;
    // Serial.println("");
  }

  if (takingPhoto == true)
  {
    // Serial.print("exposureDurationPrinting");
    idleDurationPrint = (idleDuration * 1000 - (currentTs - startTimelapseTime)) / 1000;
    exposureDurationPrint = exposureDuration;
  }
  else if (takingPhoto == false)
  {
    // Serial.print("idleDurationPrinting");
    exposureDurationPrint = (exposureDuration * 1000 - (currentTs - startTimelapseTime)) / 1000;
    idleDurationPrint = idleDuration;
  }
  // Serial.print("lastDownTime = ");
  // Serial.println(lastDownTime);
  // Serial.print("lastUpTime = ");
  // Serial.println(lastUpTime);

  // Serial.print("millis = ");
  // Serial.println(currentTs);
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

void displayBacklightControl()
{
  unsigned long currentTs = millis();

  //when taking photo than turn off displaylight after DISPLAY_BACKLIGHT_TIMES
  if (isTakingPhoto == true && isBacklightOn == true)
  {
    if (currentTs - takingPhotoBacklightControlTimes >= (DISPLAY_BACKLIGHT_TIMES * 1000))
    {
      lcd.noBacklight();
      isBacklightOn = false;
    }
  }
  else if (isTakingPhoto == false)
  {
    lcd.backlight();
    isBacklightOn = true;
    takingPhotoBacklightControlTimes = millis();
  }

  //when display idling than turn off displaylight after DISPLAY_BACKLIGHT_TIMES
  // if (currentTs - (DISPLAY_BACKLIGHT_TIMES * 1000) >= displayIdlingControlTimes)
  // {
  //   isDisplayIdling = true;
  //   if (isDisplayIdling == true && isBacklightOn == true)
  //   {
  //     lcd.noBacklight();
  //     isBacklightOn = false;
  //   }
  // }
  // else if (isDisplayIdling == false)
  // {
  //   lcd.backlight();
  //   isBacklightOn = true;
  //   displayIdlingControlTimes = millis();

  //   // when display not idle than define the display when is idling.
  //   long idlingState;
  //   long oldIdlingState;
  //   oldIdlingState = idlingState;
  //   idlingState = myEnc.read();
  //   if (idlingState != oldIdlingState)
  //   {
  //     isDisplayIdling = false;
  //     myEnc.write(oldIdlingState);
  //   }
  // }

  // Serial.print("isTakingPhoto = ");
  // Serial.println(isTakingPhoto? "Ture" : "False");
  // Serial.print("isBacklightOn = ");
  // Serial.println(isBacklightOn ? "Ture" : "False");
  // Serial.print("millis = ");
  // Serial.println(currentTs);
  // Serial.print("takingPhotoBacklightControlTimes = ");
  // Serial.println(takingPhotoBacklightControlTimes);
}

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(Step_Pin, OUTPUT);
  pinMode(Dir_Pin, OUTPUT);
  pinMode(Button, INPUT_PULLUP);

  // pinMode(PIN_LED, OUTPUT);

  button.attachLongPressStart(takePhotocontrol);
  button.attachClick(nextMeun);

  myEnc.write(10);

  lcd.begin(16, 2);
  lcd.backlight();
  // _LcdSetup();
  Serial.println("LcdSetup--Done");
  TMCstepperSetup();
  Serial.println("TMCstepperSetup--Done");
}

void loop()
{
  currentTimesMillis = millis();
  currentTimeMicros = micros();
  button.tick();
  displayBacklightControl();
  stepperMotorControl();
  if (!wasMeunUpdated)
  {
    updateMeun();
  }

  if (isTakingPhoto)
  {
    if (currentTimesMillis - updateMeunTime >= 1000) //update meun each second, when taking photo
    {
      updateMeunTime = millis();
      updateMeun();

      // Serial.println("---------taking photo debug---------");
      // Serial.print("idleDuration = ");
      // Serial.println(idleDuration);
      // Serial.print("exposureDuration = ");
      // Serial.println(exposureDuration);
      // Serial.print("lastTakePhotoTime = ");
      // Serial.println(lastTakePhotoTime);
      // Serial.print("lastIdleTime = ");
      // Serial.println(lastIdleTime);
      // Serial.print("idleDurationPrint = ");
      // Serial.println(idleDurationPrint);
      // Serial.print("exposureDurationPrint = ");
      // Serial.println(exposureDurationPrint);
      // Serial.print("millis = ");
      // Serial.println(currentTimesMillis);
      // Serial.print("startTimelapseTime = ");
      // Serial.println(startTimelapseTime);
    }
    Timelapse();
  }
  else
  {
    timeChange();
  }
}
