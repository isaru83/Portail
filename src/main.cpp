#include "Gate.h"
#include "Menu.h"

#define START_PIN 3

#define UP_BUTTON_PIN     10
#define DOWN_BUTTON_PIN   11
#define LEFT_BUTTON_PIN   12
#define RIGHT_BUTTON_PIN   9

Gate gate;
RTC_DS3231 rtc;
MenuManager *menu = new MenuManager(&gate);
FrameRate framerate(5);
StopWatch buttonClock(150);

void buttonManager()
{
  if(buttonClock.getTrigg())
  {
    if(!digitalRead(UP_BUTTON_PIN))
      menu->upInput();
    else if(!digitalRead(DOWN_BUTTON_PIN))
      menu->downInput();
    else if(!digitalRead(LEFT_BUTTON_PIN))
      menu->leftInput();
    else if(!digitalRead(RIGHT_BUTTON_PIN))
      menu->validInput();
    buttonClock.reinit();
  }
}

void getStart()
{
  gate.start();
}

void setup()
{
  while(!rtc.begin()){}
  pinMode(CELL_PIN,INPUT);
  pinMode(UP_BUTTON_PIN,INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN,INPUT_PULLUP);
  pinMode(LEFT_BUTTON_PIN,INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN,INPUT_PULLUP);
  pinMode(BLINKER_PIN,OUTPUT);
  pinMode(LIGHT_PIN,OUTPUT);
  digitalWrite(BLINKER_PIN,LOW);
  digitalWrite(LIGHT_PIN,LOW);
  Serial.begin(9600);
   //attachInterrupt(1,getStart,RISING);
  gate.init();
  menu->setRtcPtr(&rtc);
  menu->create();
}

void loop() 
{
  while(Serial.available())
  {
    char c = Serial.read();
    if(c=='o' && menu->isRoot())
    {
      gate.start();
      Serial.write("start OK!\n");
    }
    if(c=='z')
      menu->upInput();
    if(c=='s')
      menu->downInput();
    if(c=='q')
      menu->leftInput();
    if(c=='d')
      menu->validInput();
  }
  buttonManager();
  gate.execute();
  menu->display();
  framerate.run();
}

