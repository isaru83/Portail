#include "Menu.h"

MenuItem::MenuItem(const char *name, ButtonInput *button) :_parent(NULL), _button(button),_name(NULL)
{
  _name = (char*)calloc(16, sizeof(char));
  if (_name != NULL) 
    charCpy((char*)name, _name);
}

SubMenu::SubMenu(const char *name, ButtonInput *button) : MenuItem(name, button), _nbChild(0), _cursorPos(0) 
{
  for (uint8_t i = 0; i < NB_CHILD_MAX; i++) 
    _children[i] = NULL;
}

bool SubMenu::addItem(MenuItem *item) 
{
  if (_nbChild < NB_CHILD_MAX) 
  {
    _children[_nbChild++] = item;
    item->setParent(this);
    item->setLcd(_lcd);
    return true;
  }
  return false;
}

MenuItem *SubMenu::getChild(uint8_t childId)
{
  if(childId < _nbChild)
    return _children[childId];
  return nullptr;
}

MenuItem* SubMenu::execute() 
{
  _lcd->setRow(_children[_cursorPos]->getName(), 0);
  if (_cursorPos < _nbChild - 1)
    _lcd->setRow(_children[_cursorPos + 1]->getName(), 1);
  else
    _lcd->setRow("", 1);

  if (_button->up && _cursorPos > 0)
    _cursorPos--;
  else if (_button->down && _cursorPos < _nbChild - 1)
    _cursorPos++;
  else if (_button->right) 
    return _children[_cursorPos];
  else if (_button->left && _parent != NULL)
  {
    _cursorPos = 0;
    return _parent;
  }
  return this;
}

MenuItem* MenuAction::execute() 
{
  if (_manager && _function) 
  {
    MenuItem *menu = nullptr;
    menu = (_manager->*_function)();
    if(menu != nullptr)
      return menu;
  }
  if (_button->left) 
    return _parent;
  return this;
}

MenuItem* RootMenu::execute() 
{
  char buf[16]={""};
  if (_button->right && _manager->getGatePtr()->getState() == IDLE)
  {
    _paramReinit = true;
    return _children[0];
  }
  else if(_button->up && _manager->getGatePtr()->getState() == IDLE)
    _manager->getGatePtr()->start();

  strcpy_P(buf,RUN_DYSPLAY_MODEL);
  _lcd->setRow(buf, 0);
  char state = _gatePtr->getState();
  /*if(_gatePtr->getCellState())
  {
    strcpy_P(buf,MENU_CELL_MSG);
    _lcd->setRow(buf,1);
  }
  else*/ if (state == IDLE)
  {
    uint8_t hour=0, minute=0;
    _rtc->getTime(&hour,&minute);
    if(_rtc->alarmIsActivated())
      sprintf(buf,"Att Cmd   *%02d:%02d",hour,minute);
    else
      sprintf(buf,"Att Cmd    %02d:%02d",hour,minute);
    _lcd->setRow(buf, 1);
  }
  else if (state == OPEN)
  {
    strcpy_P(buf,RUN_DYSPLAY_OPEN);
    _lcd->setRow(buf, 1);
  }
  else if (state == CLOSE)
  {
    strcpy_P(buf,RUN_DYSPLAY_CLOSE);
    _lcd->setRow(buf, 1);
  }
  else if (state == PAUSE)
  {
    strcpy_P(buf,RUN_DYSPLAY_PAUSE);
    sprintf(buf+5," %ds",_gatePtr->getPauseRemainTime()+1);
    _lcd->setRow(buf, 1);
  }
  else if(state == OPEN_WARNING || state == CLOSE_WARNING)
  {
    strcpy_P(buf,MENU_TMP_PREAV);
    sprintf(buf+7," %ds",_gatePtr->getWarningRemainTime()+1);
    _lcd->setRow(buf, 1);
  }

  if(_paramReinit)
  {
    _manager->reinitParam();
    _paramReinit = false;
  }
  return this;
}

MenuManager::MenuManager(Gate *gatePtr) :_rtcPtr(NULL),_rtc(NULL), _rootMenu(NULL), _curMenu(NULL), _gatePtr(gatePtr)
                                        , _learningState(LEARNING_STOP),_hourSetting(0), _minuteSetting(0)
                                        ,_idAlarmMsg(0),_buzzer(BUZZER_TIME,BUZZ_PIN),_remoteClock(10000)
{
  clearButton();
  _idAlarmMsg = random(0,10);
  _remoteClock.reset();
}

void MenuManager::create() 
{
  char buf[16];
  _rtc = new AlarmClock(_rtcPtr,REFRESH_RTC_TIME);
  _lcd = new LcdDisplay(LCD_ADDR,LCD_LINE_SIZE,2);
  _paramCpy = *_gatePtr->getParamPtr();
  RootMenu* root = new RootMenu(_gatePtr, &_button,this);
  root->setLcd(_lcd); 
  root->setRtcPtr(_rtc);
  _rootMenu = _curMenu = root;
  SubMenu *menu = new SubMenu("Menu", &_button);
  root->addItem(menu);
  root->addItem(new MenuAction("",&MenuManager::AlarmMsg, this, &_button));
  strcpy_P(buf,MENU_PARAMETRE);
  SubMenu* parametre = new SubMenu(buf, &_button);
  menu->addItem(parametre);
  strcpy_P(buf,MENU_CONFIG);
  SubMenu* configuration = new SubMenu(buf, &_button);
  parametre->addItem(configuration);
  strcpy_P(buf,MENU_TMP_PAUSE);
  configuration->addItem(new MenuAction(buf, &MenuManager::setPauseTime, this, &_button));
  strcpy_P(buf,MENU_TMP_DECAL);
  configuration->addItem(new MenuAction(buf, &MenuManager::setOffsetTime, this, &_button));
  strcpy_P(buf,MENU_TMP_PREAV);
  configuration->addItem(new MenuAction(buf, &MenuManager::setWarningTime, this, &_button));
  strcpy_P(buf,MENU_CMD_TYPE);
  configuration->addItem(new MenuAction(buf, &MenuManager::setCmdType, this, &_button));
  strcpy_P(buf,MENU_MOTEUR);
  SubMenu* moteur = new SubMenu(buf, &_button);
  parametre->addItem(moteur);
  strcpy_P(buf,MENU_GV);
  moteur->addItem(new MenuAction(buf, &MenuManager::setHighSpeed, this, &_button));
  strcpy_P(buf,MENU_PV);
  moteur->addItem(new MenuAction(buf, &MenuManager::setLowSpeed, this, &_button));
  strcpy_P(buf,MENU_RALENT);
  moteur->addItem(new MenuAction(buf, &MenuManager::setSlowRatio, this, &_button));
  strcpy_P(buf,MENU_M1_O_POS);
  moteur->addItem(new MenuAction(buf, &MenuManager::setM1OpenPos, this, &_button));
  strcpy_P(buf,MENU_M1_F_POS);
  moteur->addItem(new MenuAction(buf, &MenuManager::setM1CLosePos, this, &_button));
  strcpy_P(buf,MENU_M2_O_POS);
  moteur->addItem(new MenuAction(buf, &MenuManager::setM2OpenPos, this, &_button));
  strcpy_P(buf,MENU_M2_F_POS);
  moteur->addItem(new MenuAction(buf, &MenuManager::setM2CLosePos, this, &_button));
  strcpy_P(buf,MENU_SECURITE);
  parametre->addItem(new MenuAction(buf, &MenuManager::setSafe, this, &_button));
  strcpy_P(buf,MENU_TELECO);
  SubMenu* remote = new SubMenu(buf, &_button);
  parametre->addItem(remote);
  strcpy_P(buf,MENU_NOUV_TELECO);
  remote->addItem(new MenuAction(buf, &MenuManager::newRemote, this, &_button));
  strcpy_P(buf,MENU_APPRENT);
  parametre->addItem(new MenuAction(buf, &MenuManager::learning, this, &_button));
  strcpy_P(buf,MENU_SAUVEGARDE);
  parametre->addItem(new MenuAction(buf, &MenuManager::saveParam, this, &_button));
  strcpy_P(buf,MENU_REVEIL);
  SubMenu* alarmClk = new SubMenu(buf, &_button);
  menu->addItem(alarmClk);
  strcpy_P(buf,MENU_REVEIL_HEURE);
  alarmClk->addItem(new MenuAction(buf,&MenuManager::clock,this,&_button));
  strcpy_P(buf,MENU_REVEIL_ACTIVER_ALARM);
  alarmClk->addItem(new MenuAction(buf,&MenuManager::AlarmActivate,this,&_button));
  strcpy_P(buf,MENU_REVEIL_REGLER_ALARM);
  alarmClk->addItem(new MenuAction(buf,&MenuManager::setAlarm,this,&_button));
  strcpy_P(buf,MENU_REVEIL_REGLER_HEURE);
  alarmClk->addItem(new MenuAction(buf,&MenuManager::setClock,this,&_button));
}

void MenuManager::clearButton() 
{
  memset(&_button,0,sizeof(ButtonInput));
}

void MenuManager::display() 
{
  if(_rtc->alarmTrigger())
  {
    _gatePtr->start();
    RootMenu *rootCast = static_cast<RootMenu*>(_rootMenu);
    _curMenu = rootCast->getChild(1);
    _rtc->alarmActivate(false);
  }
  _curMenu = _curMenu->execute();
  _lcd->display();
  clearButton();
}

void MenuManager::buzzerOn(bool activate)
{
    _buzzer.turnOn(activate);
}


LcdDisplay::LcdDisplay(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows): _lcd(lcd_Addr,lcd_cols,lcd_rows)
                                                                           ,_refreshFlag(false)
{
  _lcd.init();
  _lcd.display();
  _lcd.backlight();
  _line[0][0] = _line[1][0] = 'a';
}

void LcdDisplay::setRow(const char *str,uint8_t row)
{
  if(strcmp(str,_line[row]))
  {
    strcpy(_line[row],str);
    _refreshFlag = true;
  }
}

void LcdDisplay::display()
{
  if( _refreshFlag)
  {
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print(_line[0]);
    _lcd.setCursor(0, 1);
    _lcd.print(_line[1]);
    _refreshFlag = false;
  }
}

AlarmClock::AlarmClock(RTC_DS3231 *rtc,unsigned long refreshTime): _refresh(refreshTime), _rtc(rtc),_temp(0.0)
                                                                   ,_alarmHour(0),_alarmMinute(0),_alarmActivated(false)
{
  _time = _rtc->now();
  _refresh.start();
}

void AlarmClock::getTime(uint8_t *hour,uint8_t *minute)
{
  if(_refresh.getTrigg())
  {
    _time = _rtc->now();
    *hour =  _time.hour();
    *minute = _time.minute();
    _temp = _rtc->getTemperature();
    _refresh.reinit();
  }
  else
  {
    *hour = _time.hour();
    *minute = _time.minute();
  }
}

void AlarmClock::getTime(DateTime *time)
{
  if(_refresh.getTrigg())
  {
    _time = _rtc->now();
    _temp = _rtc->getTemperature();
    _refresh.reinit();
  }
  *time = _time;
}

bool AlarmClock::alarmTrigger()
{
  if(_refresh.getTrigg())
  {
    _time = _rtc->now();
    _temp = _rtc->getTemperature();
    _refresh.reinit();
  }
  if(_alarmActivated && _time.minute() == getAlarmMinute() 
                            && _time.hour() == getAlarmHour() && _time.second() == 0)
    return true;
  return false;
}



