#include "Menu.h"

MenuItem *MenuManager::setPauseTime() 
{
  char buf[16];
  sprintf(buf, "%ds", _paramCpy.pauseTime / 1000);
  _lcd->setRow(buf, 1);

  if (_button.up && _paramCpy.pauseTime > 0)
    _paramCpy.pauseTime -= 1000;
  if (_button.down && _paramCpy.pauseTime < 60000)
    _paramCpy.pauseTime += 1000;
  return nullptr;
}

MenuItem *MenuManager::setOffsetTime() 
{
  char buf[16]={""};
  if(_paramCpy.offsetTime > 1000)
  {
    sprintf(buf, "%ds", (_paramCpy.offsetTime / 1000));
    _lcd->setRow(buf, 1);

    if (_button.up && _paramCpy.offsetTime > 0)
      _paramCpy.offsetTime -= 1000;
    if (_button.down && _paramCpy.offsetTime < 10000)
      _paramCpy.offsetTime += 1000;
  }
  else if(_paramCpy.offsetTime == 1000)
  {
    sprintf(buf, "%ds", (_paramCpy.offsetTime / 1000));
    _lcd->setRow(buf, 1);

    if (_button.up && _paramCpy.offsetTime > 0)
      _paramCpy.offsetTime -= 100;
    if (_button.down && _paramCpy.offsetTime < 10000)
      _paramCpy.offsetTime += 1000;
  }
  else if(_paramCpy.offsetTime < 1000)
  {
    float x = (float)_paramCpy.offsetTime / (float)(1000);
    String offset(x,1);
    sprintf(buf,"%ss",offset.c_str());
    _lcd->setRow(buf, 1);

    if (_button.up && _paramCpy.offsetTime > 0)
      _paramCpy.offsetTime -= 100;
    if (_button.down && _paramCpy.offsetTime < 10000)
      _paramCpy.offsetTime += 100;
  }
  return nullptr;
}

MenuItem *MenuManager::setWarningTime()
{
  char buf[16];
  unsigned int x = _paramCpy.warningTime / 1000;
  if(x==0)
    sprintf(buf, "Non");
  else
    sprintf(buf, "%ds",x);
  _lcd->setRow(buf, 1);

  if (_button.up && _paramCpy.warningTime > 0)
    _paramCpy.warningTime -= 1000;
  if (_button.down && _paramCpy.warningTime < 5000)
    _paramCpy.warningTime += 1000;
  return nullptr;
}

MenuItem *MenuManager::setCmdType() 
{
   char buf[16];
   if (_paramCpy.type == AUTO)
   { 
     strcpy_P(buf,MENU_CMD_AUTO);
     _lcd->setRow(buf, 1);
   }
   else if (_paramCpy.type == REARM)
  {
    strcpy_P(buf,MENU_CMD_REARM);
     _lcd->setRow(buf, 1);
  }
   if (_button.up && _paramCpy.type > 0)
     _paramCpy.type -= 1;
   if (_button.down && _paramCpy.type < 1)
     _paramCpy.type += 1;
   return nullptr;
}

MenuItem *MenuManager::setHighSpeed() 
{
  char buf[16];
  sprintf(buf, "%d", _paramCpy.highSpeed);
  _lcd->setRow(buf, 1);

  if (_button.up && _paramCpy.highSpeed > 0)
    _paramCpy.highSpeed -= 1;
  if (_button.down && _paramCpy.highSpeed < 100)
    _paramCpy.highSpeed += 1;
  return nullptr;
}

MenuItem *MenuManager::setLowSpeed() 
{
  char buf[16];
  sprintf(buf, "%d", _paramCpy.lowSpeed);
  _lcd->setRow(buf, 1);

  if (_button.up && _paramCpy.lowSpeed > 0)
    _paramCpy.lowSpeed -= 1;
  if (_button.down && _paramCpy.lowSpeed < _paramCpy.highSpeed)
    _paramCpy.lowSpeed += 1;
  return nullptr;
}

MenuItem *MenuManager::setSlowRatio() 
{
   char buf[16];
   sprintf(buf, "%d\%", _paramCpy.slowdownDist);
   _lcd->setRow(buf, 1);

   if (_button.up && _paramCpy.slowdownDist > 0)
     _paramCpy.slowdownDist -= 5;
   if (_button.down && _paramCpy.slowdownDist < 100)
     _paramCpy.slowdownDist += 5;
   return nullptr;
}

MenuItem *MenuManager::setM1OpenPos() 
{
  char buf[16];
  sprintf(buf, "%d", _paramCpy.M1OpenPos - SERVO_MIN);
  _lcd->setRow(buf, 1);

  if (_button.up && _paramCpy.M1OpenPos > SERVO_MIN)
    _paramCpy.M1OpenPos -= 5;
  if (_button.down && _paramCpy.M1OpenPos < SERVO_MAX)
    _paramCpy.M1OpenPos += 5;
  return nullptr;
}

MenuItem *MenuManager::setM1CLosePos() 
{
  char buf[16];
  sprintf(buf, "%d", _paramCpy.M1ClosePos - SERVO_MIN);
  _lcd->setRow(buf, 1);

  if (_button.up && _paramCpy.M1ClosePos > SERVO_MIN)
    _paramCpy.M1ClosePos -= 5;
  if (_button.down && _paramCpy.M1ClosePos < SERVO_MAX)
    _paramCpy.M1ClosePos += 5;
  return nullptr;
}

MenuItem *MenuManager::setM2OpenPos() 
{
   char buf[16];
   sprintf(buf, "%d", _paramCpy.M2OpenPos);
   _lcd->setRow(buf, 1);

   if (_button.up && _paramCpy.M2OpenPos > SERVO_MIN)
     _paramCpy.M2OpenPos -= 5;
   if (_button.down && _paramCpy.M2OpenPos < SERVO_MAX)
     _paramCpy.M2OpenPos += 5;
   return nullptr;
}

MenuItem *MenuManager::setM2CLosePos() 
{
  char buf[16];
  sprintf(buf, "%d", _paramCpy.M2ClosePos);
  _lcd->setRow(buf, 1);

  if (_button.up && _paramCpy.M2ClosePos > SERVO_MIN)
    _paramCpy.M2ClosePos -= 5;
  if (_button.down && _paramCpy.M2ClosePos < SERVO_MAX)
    _paramCpy.M2ClosePos += 5;
  return nullptr;
}

MenuItem *MenuManager::setSafe() 
{
  char buf[16];
  if(_paramCpy.cellIsActivate)
    sprintf(buf,"oui");
  else
    sprintf(buf,"non");
  _lcd->setRow(buf,1);

  if(_button.up)
    _paramCpy.cellIsActivate = true;
  if(_button.down)
    _paramCpy.cellIsActivate = false;
  return nullptr;
}

MenuItem *MenuManager::saveParam() 
{
  char buf[16];
  strcpy_P(buf,MENU_BOUTON_SAUVE);
  _lcd->setRow(buf, 1);
  if (_button.right) 
  {
    _gatePtr->setParam(_paramCpy);
    return _curMenu->getParent();
  }
  return nullptr;
}

MenuItem *MenuManager::learning() 
{
  char buf[16]={""};
  strcpy_P(buf,MENU_APPRENT_BOUTON);
  _lcd->setRow(buf, 1);
  if (_learningState == LEARNING_STOP)
  {
    sprintf(buf,"Commencer");
    _lcd->setRow(buf, 0);
    if (_button.right) 
      _learningState = LEARNING_M1_OPEN;
  }
  else if (_learningState == LEARNING_M1_OPEN) 
  {
    strcpy_P(buf,MENU_M1_O_POS);
    sprintf(buf+10," %d",_gatePtr->getM1Pos());
    _lcd->setRow(buf, 0);
    if (_button.up) 
      _gatePtr->M1Moove(LEARNING_SPEED);
    else if (_button.down) 
      _gatePtr->M1Moove(-LEARNING_SPEED);
    else if (_button.right) 
    {
      _paramCpy.M1OpenPos =  _gatePtr->getM1Pos();
      _learningState = LEARNING_M2_OPEN;
    }
  } 
  else if (_learningState == LEARNING_M2_OPEN) 
  {
    strcpy_P(buf,MENU_M2_O_POS);
    sprintf(buf+10," %d",_gatePtr->getM2Pos());
    _lcd->setRow(buf, 0);
    if (_button.up) 
      _gatePtr->M2Moove(-LEARNING_SPEED);
    else if (_button.down) 
      _gatePtr->M2Moove(LEARNING_SPEED);
    else if (_button.right) 
    {
      _paramCpy.M2OpenPos =  _gatePtr->getM2Pos();
      _learningState = LEARNING_M2_CLOSE;
    }
  } 
  else if (_learningState == LEARNING_M2_CLOSE) 
  {
    strcpy_P(buf,MENU_M2_F_POS);
    sprintf(buf+10," %d",_gatePtr->getM2Pos());
    _lcd->setRow(buf, 0);
    if (_button.up) 
      _gatePtr->M2Moove(-LEARNING_SPEED);
    else if (_button.down) 
      _gatePtr->M2Moove(LEARNING_SPEED);
    else if (_button.right) 
    {
      _paramCpy.M2ClosePos =  _gatePtr->getM2Pos();
      _learningState = LEARNING_M1_CLOSE;
    }
  } 
  else if (_learningState == LEARNING_M1_CLOSE) 
  {
    strcpy_P(buf,MENU_M1_F_POS);
    sprintf(buf+10," %d",_gatePtr->getM1Pos());
    _lcd->setRow(buf, 0);
    if (_button.up) 
      _gatePtr->M1Moove(-LEARNING_SPEED);
    else if (_button.down) 
      _gatePtr->M1Moove(LEARNING_SPEED);
    else if (_button.right) 
    {
      _paramCpy.M1ClosePos =  _gatePtr->getM1Pos();
      _learningState = LEARNING_END;
    }
  }
  else if(_learningState == LEARNING_END)
  {
    _gatePtr->setParam(_paramCpy);
    _learningState = LEARNING_STOP;
    return _curMenu->getParent();
  }

  if (_button.left)
  {
    _learningState = LEARNING_STOP;
    reinitParam();
    return _curMenu->getParent();
  }
  return nullptr;
}

MenuItem *MenuManager::clock()
{
  char buf[16]={""};
  DateTime  now;
  _rtc->getTime(&now);
  String temp(_rtc->getTemperature(),1);
  sprintf(buf,"%02d/%02d/%02d  %sC",now.day(),now.month(),now.year()-2000,temp.c_str());
  _lcd->setRow(buf,0);
  if(_rtc->alarmIsActivated())
    sprintf(buf,"     *%02d:%02d",now.hour(),now.minute());
  else
    sprintf(buf,"      %02d:%02d",now.hour(),now.minute());
  _lcd->setRow(buf,1);
  return nullptr;
}

MenuItem *MenuManager::setClock()
{
  char buf[16];
  sprintf(buf,"%02d:%02d",_hourSetting,_minuteSetting);
  _lcd->setRow(buf,1);
  if (_button.up )
  {
    if(_hourSetting + 1 < 24)
        _hourSetting++;
    else
      _hourSetting = 0;
  }
  else if (_button.down)
  {
    if(_minuteSetting + 1 < 60)
      _minuteSetting++;
    else
      _minuteSetting = 0;
  }
  else if(_button.right)
  {
    DateTime now(0,0,0,_hourSetting,_minuteSetting);
    _rtc->setTime(now);
    return _curMenu->getParent();
  }
  return nullptr;
}

MenuItem *MenuManager::setAlarm()
{
  char buf[16];
  uint8_t hour = _rtc->getAlarmHour();
  uint8_t minute = _rtc->getAlarmMinute();
  sprintf(buf,"%02d:%02d",hour,minute);
  _lcd->setRow(buf,1);
  if (_button.up )
  {
    if(hour + 1 < 24)
      _rtc->setAlarmHour(++hour);
    else
      _rtc->setAlarmHour(0);
  }
  if (_button.down)
  {
    if(minute + 1 < 60)
      _rtc->setAlarmMinute(++minute);
    else
      _rtc->setAlarmMinute(0);
  }
  return nullptr;
}

MenuItem *MenuManager::AlarmActivate()
{
  char buf[16];
  if(_rtc->alarmIsActivated())
    sprintf(buf,"oui");
  else
    sprintf(buf,"non");
  _lcd->setRow(buf,1);

  if(_button.up)
    _rtc->alarmActivate(true);
  if(_button.down)
    _rtc->alarmActivate(false);
  return nullptr;
}

MenuItem *MenuManager::AlarmMsg()
{
  _buzzer.run();
  if(_idAlarmMsg<10)
  {
    char buf[16];
    const char* msg = (const char*)pgm_read_word(&(ALARM_MSG_TAB[_idAlarmMsg]));
    strcpy_P(buf, msg);
    _lcd->setRow(buf,0);
    sprintf(buf,"OT45 BLQ Fermer");
    _lcd->setRow(buf,1);
    _buzzer.turnOn(true);
  }

  if(_button.right)
  {
    _buzzer.turnOn(false);
  }
  if(_button.left)
  {
    _idAlarmMsg = random(0,10);
    _buzzer.turnOn(false);
    return _rootMenu;
  }
  return nullptr;
}

MenuItem *MenuManager::newRemote()
{
  char buf[16];
  uint8_t sec = _remoteClock.remainTimeSecond();
  if(!_remoteClock.isStarted())
  {
    digitalWrite(REMOTE_RCV_PIN,HIGH);
    delay(100);
    digitalWrite(REMOTE_RCV_PIN,LOW);
  }
  _remoteClock.start();
  if(sec > 0)
  {
    _remoteClock.getTrigg();
    if(sec == 1)
    {
      _remoteClock.reset();
      return _curMenu->getParent();
    }
    sprintf(buf,"%ds",sec);
    _lcd->setRow(buf,1);
  }
  
  if(_button.left)
  {
    digitalWrite(REMOTE_RCV_PIN,HIGH);
    delay(100);
    digitalWrite(REMOTE_RCV_PIN,LOW);
    _remoteClock.reset();
  }
  return nullptr;
}

void floatToStr(char *str, float x,unsigned int precision)
{
    char expStr[8],mantStr[8];
    int exponent =(int)x;
    float mantisse = x - exponent;
    sprintf(expStr,"%d",exponent);
    sprintf(mantStr,"%d",(int)(mantisse*precision));
    sprintf(str,"%s.%s",expStr,mantStr);
}

void charCpy(char *src, char *dest) 
{
  uint8_t i;
  for (i = 0; i < 16 && src[i] != '\0'; i++) 
    dest[i] = src[i];
  dest[i] = '\0';
}
