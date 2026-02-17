#include "Gate.h"

Gate::Gate(): _state(IDLE), _cell(), _startFlag(false),_pauseClock(0),_offsetClock(0),_warningClock(0), _isOpen(false)
                          ,_warnLight(BLINK_TIME,BLINKER_PIN)
{
  _param.pauseTime = 5000;
  _param.cellIsActivate = false;
  _param.M1ClosePos = 545;
  _param.M2ClosePos = 1500;
  _param.M1OpenPos = 1500;
  _param.M2OpenPos = 545;
  _param.offsetTime = 1000;
  _param.warningTime = 2000;
  _param.slowdownDist = 85;
  _param.highSpeed = 5;
  _param.lowSpeed = 1;
  _param.type = AUTO;
}

void Gate::init()
{
  //_M1.writeMicroseconds(_M1Pos);
  _M1.attach(M1_PIN);
  _M2.attach(M2_PIN);
  _param = loadParam();
  M1Moove(_param.M1ClosePos);
  M2Moove(_param.M2ClosePos);
  _M1Pos = getM1Pos();
  _M2Pos = getM2Pos();
  _pauseClock.setDelay(_param.pauseTime);
  _offsetClock.setDelay(_param.offsetTime);
  _warningClock.setDelay(_param.warningTime);
  setMotorsSlowPos();
}

void Gate::lightOn(bool activate)
{
  if(activate)
    digitalWrite(LIGHT_PIN,HIGH);
  else
    digitalWrite(LIGHT_PIN,LOW);
}

bool Gate::open()
{
    bool ret = true;
    if(_M1Pos <= _param.M1OpenPos)
    {
        ret = false;
        _M1Pos += speedManager(1);
        _M1.writeMicroseconds(_M1Pos);
        _offsetClock.start();
    }
    if(_M2Pos >= _param.M2OpenPos)
    {
      ret = false;
      if(_offsetClock.getTrigg())
      {
         _M2Pos -= speedManager(2);
        _M2.writeMicroseconds(_M2Pos);
      }
    }
    return ret;
}

bool Gate::pause()
{
    if(_param.type != REARM)
    {
        if(!_pauseClock.getTrigg())
          return false;
    }
    return true;
}

bool Gate::warning()
{
  if(_param.warningTime > 0)
  {
    _warningClock.start();
    if(!_warningClock.getTrigg())
      return false;
  }
  return true;
}

bool Gate::close()
{
    bool ret = true;
    if(_M2Pos <= _param.M2ClosePos)
    {
        ret = false;
        _M2Pos += speedManager(2);
        _M2.writeMicroseconds(_M2Pos);
        _offsetClock.start();
    }
    if(_M1Pos >= _param.M1ClosePos)
    {
        ret = false;
        if(_offsetClock.getTrigg())
        {
          _M1Pos -= speedManager(1);
          _M1.writeMicroseconds(_M1Pos);
        }
    }
    return ret;
}

bool Gate::run()
{
    bool ret = false;
    if(_state == OPEN_WARNING)
    {
      lightOn(true);
      _warnLight.turnOn(true);
      if(warning())
        _state = OPEN;
    }
    else if(_state == OPEN)
    {
        _warningClock.reset();
        if(open())
        {
            _isOpen = true;
            if(_param.type == AUTO)
            {
                _state = PAUSE;
                _pauseClock.start();
                _warnLight.turnOn(false);
            }
            else
            {
                _state = IDLE;
                lightOn(false);
                _warnLight.turnOn(false);
            }
        }
    }
    else if(_state == PAUSE)
    {
        _offsetClock.reset();
        if(pause() || _param.type == REARM)
        {
          if(_param.warningTime > 0)
            _state = CLOSE_WARNING;
          else
          {
            lightOn(true);
            _warnLight.turnOn(true);
            _state = CLOSE;
          }
        }
    }
    else if(_state == CLOSE_WARNING)
    {
      _warnLight.turnOn(true);
      if(warning())
        _state = CLOSE;
    }
    else if(_state == CLOSE)
    {
      _warningClock.reset();
      _pauseClock.reset();
      _isOpen = false;
      if(_param.cellIsActivate && _cell.isActive())
        _state = OPEN;
      if(close())
      {
        _offsetClock.reset();
        _state = IDLE;
        lightOn(false);
        _warnLight.turnOn(false);
      }
    }

    if(_state != IDLE)
        ret = true;
    return ret;
}

void Gate::execute()
{
    if(isStarted())
    {
        if(_param.type == AUTO)
        {
          if(_state == PAUSE)
            _pauseClock.reinit();

          if(_param.warningTime > 0 && _state == IDLE)
            _state = OPEN_WARNING;
          else
          {
            _state = OPEN;
            lightOn(true);
            _warnLight.turnOn(true);
          }

          
        }
        else if(_param.type == REARM)
        {
          if(_isOpen)
          {
            lightOn(true);
            _warnLight.turnOn(true);
            if(_param.warningTime > 0)
              _state = CLOSE_WARNING;
            else
              _state = CLOSE;
          }
          else 
          {
            lightOn(true);
            _warnLight.turnOn(true);
            if(_param.warningTime > 0 && _state == IDLE)
              _state = OPEN_WARNING;
            else
              _state = OPEN;
          }
        }
    }
    if(_state != IDLE)
        run();
    _warnLight.run();
}

bool Gate::isStarted()
{
    if(_startFlag)
    {
        _startFlag = false;
        return true;
    }
    return false;
}

void Gate::saveParam()
{
  EEPROM.put(PARAM_ADDR, _param);
}

Param Gate::loadParam()
{
  Param ret;
  EEPROM.get(PARAM_ADDR, ret);
  return ret;
}

bool Gate::isOpen()
{
  if(_M1Pos == _param.M1OpenPos && _M2Pos == _param.M2OpenPos)
    return true;
  return false;
}

bool Gate::isInLimits(unsigned int val)
{
  if(val < SERVO_MIN || val > SERVO_MAX)
    return false;
  return true;
}

unsigned int Gate::absolute(int val)
{
  if(val>=0)
    return (unsigned int)val;
  return (unsigned int)(val * -1);
}

unsigned int Gate::getRatio(unsigned int val, unsigned int ratio)
{
   return (unsigned int)((unsigned long)val * ratio / 100);
}

unsigned int Gate::speedManager(uint8_t motorNum)
{
  if(motorNum == 1)
  {
    if((_state == OPEN && _M1Pos >= _motorSlowPos.M1_Open) || (_state == CLOSE && _M1Pos <= _motorSlowPos.M1_Close))
      return _param.lowSpeed;
    return _param.highSpeed;
  }
  else if(motorNum == 2)
  {
    if((_state == OPEN && _M2Pos <= _motorSlowPos.M2_Open) || (_state == CLOSE && _M2Pos >= _motorSlowPos.M2_Close))
      return _param.lowSpeed;
    return _param.highSpeed;
  }
}

void Gate::setMotorsSlowPos()
{
    unsigned int gap  = _param.M1OpenPos - _param.M1ClosePos;
    unsigned int ratio = getRatio(gap,_param.slowdownDist);
    _motorSlowPos.M1_Open = _param.M1ClosePos + ratio;
    _motorSlowPos.M1_Close = _param.M1OpenPos - ratio;

    gap = absolute(_param.M2ClosePos - _param.M2OpenPos);
    ratio = getRatio(gap,_param.slowdownDist);
    _motorSlowPos.M2_Open = _param.M2ClosePos - ratio;
    _motorSlowPos.M2_Close = _param.M2OpenPos + ratio;
}

void Gate::setParam(Param param)
{
    _param = param;
    setMotorsSlowPos();
    _pauseClock.setDelay(param.pauseTime);
    _warningClock.setDelay(_param.warningTime);
    _offsetClock.setDelay(_param.offsetTime);
    saveParam();
}

void Gate::M1Moove(int val)
{
  if((_M1Pos + val < SERVO_MAX) && (_M1Pos + val > SERVO_MIN))
  {
    _M1Pos += val;
    _M1.writeMicroseconds(_M1Pos);
  }
}

void Gate::M2Moove(int val)
{
  if((_M2Pos + val < SERVO_MAX) && (_M2Pos + val > SERVO_MIN))
  {
    _M2Pos += val;
    _M2.writeMicroseconds(_M2Pos);
  }
}

Blinker::Blinker(unsigned int time,uint8_t pin): _blinkClock(time),_activate(false),_pin(pin)
{
  _blinkClock.start();
}

void Blinker::turnOn(bool activate)
{
  _activate = activate;
  if(!activate)
    digitalWrite(_pin,LOW);
}

void Blinker::run()
{
  if(_activate && _blinkClock.getTrigg())
  {
    if(digitalRead(_pin) == HIGH)
      digitalWrite(_pin,LOW);
    else
      digitalWrite(_pin,HIGH);
    _blinkClock.reinit();
  }
}

PhotoCell::PhotoCell() 
{
  
}

bool PhotoCell::isActive()
{
  int x = digitalRead(CELL_PIN);
  if(x == LOW)
    return true;
  return false;
}

