#include "Timer.h"

void StopWatch::start()
{
  if(!_isStartedFlag)
  {
    _isStartedFlag = true;
    _remainTime = 0;
    _startTime = millis();
  }  
}

bool StopWatch::getTrigg()
{
  if(_isStartedFlag)
  {
    unsigned long actualTime = millis();
    _remainTime = actualTime - _startTime;
    if( _remainTime >= _delay)
    {
        //isStartedFlag = false;
        return true;
    }
  }
    return false;
}

void StopWatch::setDelay(unsigned long delay)
{
  _delay = delay;
}

unsigned int StopWatch::remainTimeSecond()
{
  if(_isStartedFlag && _delay - _remainTime>0)
    return (unsigned int)((_delay - _remainTime)/1000);   //(_remainTime/1000);
  return 0;
}


void FrameRate::run()
{
  unsigned long nowTime = millis();
  if (nowTime - _prevTime >= _time) 
    _prevTime = nowTime;
  else 
  {
    while (millis() - _prevTime < _time) {}
    _prevTime = millis();
  }
}

/*bool StopWatchus::getTrigg()
{
    unsigned long actualTime = micros();
    _remainTime = actualTime - _startTime;
    if( _remainTime >= _delay)
    {
        //isStartedFlag = false;
        return true;
    }
    return false;
}

unsigned int StopWatchus::remainTimeSecond()
{
  if(_delay - _remainTime>0)
    return (unsigned int)((_delay - _remainTime)/1000);   //(_remainTime/1000);
  return 0;
}*/
