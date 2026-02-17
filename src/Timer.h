#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "Arduino.h"

class FrameRate
{
  public:
    FrameRate(unsigned int time) : _time(time), _prevTime(0) {}
    void run();

  private:
    unsigned int _time;
    unsigned long _prevTime;
};

class StopWatch
{
  public:
    StopWatch(const unsigned long delay): _prevTime(0), _startTime(0), _delay(delay),_remainTime(0),_isStartedFlag(false) {}
    void start();
    bool getTrigg();
    void reset() {_isStartedFlag = false;_remainTime = 0;}
    void setDelay(unsigned long delay);
    void reinit() {reset();start();}
    unsigned int remainTimeSecond();
    bool isStarted() { return _isStartedFlag;}

  protected:
    unsigned long _prevTime,_startTime,_delay,_remainTime;
    bool _isStartedFlag;
};

/*class StopWatchus : public StopWatch
{
  public:
    StopWatchus(const unsigned long delayus): StopWatch(delayus) {}
    bool getTrigg();
    unsigned int remainTimeSecond();
};*/
#endif // TIMER_H_INCLUDED
