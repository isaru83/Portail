#ifndef GATE_H_INCLUDED
#define GATE_H_INCLUDED

#include "Arduino.h"
#include <Servo.h>
#include "EEPROM.h"
#include "Timer.h"

#define PARAM_ADDR 10

#define M1_PIN       5
#define M2_PIN       6
#define CELL_PIN    A7
#define BLINKER_PIN  3
#define LIGHT_PIN    4
#define LED_IR_PIN   9

#define SERVO_MAX 2400
#define SERVO_MIN  545

#define IDLE          0
#define OPEN          1
#define CLOSE         2
#define PAUSE         3 
#define OPEN_WARNING  4
#define CLOSE_WARNING 5

#define AUTO  0
#define REARM 1

#define BLINK_TIME    500

using namespace std;

typedef struct
{
    unsigned int M1OpenPos,M2OpenPos,M1ClosePos,M2ClosePos;
    unsigned int pauseTime, offsetTime, warningTime;
    unsigned int slowdownDist;
    unsigned int highSpeed, lowSpeed;
    bool cellIsActivate;
    char type;
}Param;

typedef struct
{
  unsigned int M1_Open, M2_Open, M1_Close, M2_Close;
}MotorSlowPos;

class PhotoCell
{
    public:
        PhotoCell();
        bool isActive();
};

class Blinker
{
  public:
    Blinker(unsigned int time,uint8_t pin);
    void turnOn(bool activate);
    void run();

  private:
    StopWatch _blinkClock;
    bool _activate;
    uint8_t _pin;
};

class Gate
{
    public:
        Gate();
        void init();
        void execute();
        void start() { _startFlag = true;}
        int getM1Pos() {return _M1Pos;}
        int getM2Pos() {return _M2Pos;}
        Param *getParamPtr() {return &_param;}
        const char getState() { return _state; }
        void setParam(Param param);
        void saveParam();
        Param loadParam();
        void M1Moove(const int val);
        void M2Moove(const int val);
        bool getCellState() {return _cell.isActive();}
        void lightOn(bool activate);
        //void saveM1OpenCurPos();
        //void saveM1CloseCurPos();
        //void saveM2OpenCurPos();
        //void saveM2CloseCurPos();
        unsigned int getPauseRemainTime(){ return _pauseClock.remainTimeSecond();}
        unsigned int getWarningRemainTime(){ return _warningClock.remainTimeSecond();}

    private:
        bool open();
        bool close();
        bool pause();
        bool warning();
        bool run();
        bool isStarted();
        bool isInLimits(unsigned int val);
        unsigned int absolute(int val);
        void setMotorsSlowPos();
        unsigned int speedManager(uint8_t motorNum);
        unsigned int getRatio(unsigned int val,unsigned int ratio);
        void serialIntToChar(unsigned int x);
        bool isOpen();
        void setPauseClockDelay(unsigned int delay) { _pauseClock.setDelay(delay);}
        

        char _state;
        bool _startFlag;
        StopWatch _pauseClock, _offsetClock,_warningClock;
        bool _isOpen;
        unsigned int _M1Pos, _M2Pos;
        Servo _M1,_M2;
        Param _param;
        MotorSlowPos _motorSlowPos;
        Blinker _warnLight;
        PhotoCell _cell;
};

#endif // GATE_H_INCLUDED
