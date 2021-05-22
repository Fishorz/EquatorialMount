#include <Arduino.h>
#include <OneButton.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>
#include "ConfigFile.h"
#include "LCD_setup.h"

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

int lastMeun;
int meun = 0;
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
  case 0:
  {
    lcd.setCursor(0, 0);
    lcd.print("Interval Time");
    lcd.setCursor(1, 1);
    lcd.print(idleDuration);
    lcd.print("sec");
    Serial.println("Interval Time");
    lastIdleDuration = idleDuration;
    break;
  }

  case 1:
  {
    lcd.setCursor(0, 0);
    lcd.print("Exposure Time");
    lcd.setCursor(1, 1);
    lcd.print(exposureDuration);
    lcd.print("sec");
    Serial.println("Exposure Time");
    lastExposureDuration = exposureDuration;
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
      Serial.println("Rotate On");
    }
    else
    {
      lcd.print("Rotate Stop");
      Serial.println("Rotate Stop");
    }

    break;
  }

  case 3:
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
  Serial.print("meun=");
  Serial.println(meun);
}

void nextMeun() //one click
{
  Serial.println("Short Press");
  wasMeunUpdated = false;
  // counter = 0;
  if (isTakingPhoto == false)
  {
    // Serial.println("Click");
    lcd.clear();
    meun++;
    if (meun > 2)
    {
      meun = 0;
    }
    Serial.print("meun=");
    Serial.println(meun);

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

  bool shouldDown = (currentTs >= lastUpTime + (idleDuration * 1000)) && state == 0;
  bool shouldUp = (currentTs >= lastDownTime + (exposureDuration * 1000)) && state == 1;

  delay(10);
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
  num = num / 4;
}

void timeChange()
{
  switch (meun)
  {
  case 0:
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
      Serial.print("idleDuration=");
      Serial.println(idleDuration);
    }
    break;
  }

  case 1:
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
      Serial.print("exposureDuration=");
      Serial.println(exposureDuration);
    }
    break;
  }

  case 2:
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

  Serial.println("start");
  button.attachLongPressStart(takePhotocontrol);
  button.attachClick(nextMeun);

  myEnc.write(10);

  lcd.begin(16, 2);
  lcd.backlight();
  // _LcdSetup();
}

void loop()
{
  button.tick();
  if (wasMeunUpdated == false)
  {
    updateMeun();
  }

  if (isTakingPhoto == true)
  {
    Timelapse();
  }
  else
  {
    timeChange();
  }

  // Serial.println(digitalRead(Button));
}
