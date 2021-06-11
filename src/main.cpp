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
unsigned long stepperDelayTime = 28125; //microseconds

enum Menu
{
  intervalSetup = 0,
  exposureDurationSetup,
  rotateEnable,
  takePhoto,
};

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
unsigned long startTimelapseTime = 0;
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
    Serial.println("---------taking photo debug---------");
    Serial.print("idleDuration = ");
    Serial.println(idleDuration);
    Serial.print("exposureDuration = ");
    Serial.println(exposureDuration);
    Serial.print("millis = ");
    Serial.println(millis());
    Serial.print("lastTakePhotoTime = ");
    Serial.println(lastTakePhotoTime);
    Serial.print("lastIdleTime = ");
    Serial.println(lastIdleTime);
    Serial.print("idleDurationPrint = ");
    Serial.println(idleDurationPrint);
    Serial.print("exposureDurationPrint = ");
    Serial.println(exposureDurationPrint);
    Serial.print("startTimelapseTime = ");
    Serial.println(startTimelapseTime);
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
    startTimelapseTime = millis();
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
  if (isRotating)
  {
    // Serial.print("rotating");
    digitalWrite(EN_PIN, LOW);
    // unsigned long currenTimeMicros = micros();
    if (currentTimeMicros - previousTime >= stepperDelayTime)
    {
      digitalWrite(Step_Pin, HIGH);
      previousTime = currentTimeMicros;
    }
    //delay stepperDelayTime
    if (currentTimeMicros - (previousTime + stepperDelayTime) >= stepperDelayTime)
    {
      digitalWrite(Step_Pin, LOW);
    }
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
  // unsigned long currentTs = millis();

  bool idle = (currentTimesMillis - lastIdleTime >= (idleDuration * 1000)) && state == 0;
  bool takephoto = (currentTimesMillis - lastTakePhotoTime >= (exposureDuration * 1000)) && state == 1;
  bool takingPhoto = true; // True is idlling, False is exposuring.
  // delay(10);
  // Serial.println(currentTs);
  if (takephoto || idle)
  {
    // Serial.println(shouldUp ? "UP" : "DOWN");
    if (takephoto)
    {
      lastTakePhotoTime = currentTimesMillis;
      photoNumber++;
      takingPhoto = true;
      Serial.println("takephoto = true");
      // wasMeunUpdated = false;
    }
    if (idle)
    {
      lastIdleTime = currentTimesMillis;
      takingPhoto = false;
      Serial.println("idle = true");
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
    exposureDurationPrint = (exposureDuration * 1000 - (millis() - startTimelapseTime - lastTakePhotoTime)) / 1000;
  }
  else if (takingPhoto == false)
  {
    // Serial.print("idleDurationPrinting");
    idleDurationPrint = (idleDuration * 1000 - (millis() - startTimelapseTime - lastIdleTime)) / 1000;
  }
  // Serial.print("lastDownTime = ");
  // Serial.println(lastDownTime);
  // Serial.print("lastUpTime = ");
  // Serial.println(lastUpTime);
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
  Serial.begin(BAUD_RATE);
  pinMode(Step_Pin, OUTPUT);
  pinMode(Dir_Pin, OUTPUT);
  pinMode(Button, INPUT_PULLUP);

  // pinMode(PIN_LED, OUTPUT);
  // TMCstepperSetup();
  Serial.println("TMCstepperSetup--Done");

  button.attachLongPressStart(takePhotocontrol);
  button.attachClick(nextMeun);

  myEnc.write(10);

  lcd.begin(16, 2);
  lcd.backlight();
  // _LcdSetup();
  Serial.println("LcdSetup--Done");
}

void loop()
{
  currentTimesMillis = millis();
  currentTimeMicros = micros();
  button.tick();

  if (!wasMeunUpdated)
  {
    updateMeun();
  }

  if (isTakingPhoto)
  {
    if (currentTimesMillis - updateMeunTime >= 1000)
    {
      updateMeunTime = millis();
      updateMeun();
    }
    Timelapse();
  }
  else
  {
    timeChange();
  }
  stepperMotorControl();
  //update meun each second

  // Serial.println(digitalRead(Button));
}
