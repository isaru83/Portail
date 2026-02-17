#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include "Gate.h"
#include "Timer.h"
#include "RTClib.h"
#include "String.h"

#define LCD_ADDR 0x27
#define LCD_LINE_SIZE 16

#define NB_CHILD_MAX 10
//#define LCDLINE_SIZE 16

#define UP_BUTTON    0
#define DOWN_BUTTON  1
#define LEFT_BUTTON  2
#define VALID_BUTTON 3

#define LEARNING_STOP      0
#define LEARNING_M1_OPEN   1
#define LEARNING_M1_CLOSE  2
#define LEARNING_M2_OPEN   3
#define LEARNING_M2_CLOSE  4
#define LEARNING_END       5
#define LEARNING_SPEED    10 

#define REFRESH_RTC_TIME 1000

#define NB_ALARM_MSG  10
#define BUZZER_TIME  500

#define BUZZ_PIN   A1
#define REMOTE_RCV_PIN 13

const char ALARM_MSG_1[] PROGMEM = "Le Bailli";
const char ALARM_MSG_2[] PROGMEM = "Les Regates";
const char ALARM_MSG_3[] PROGMEM = "Le Crystal";
const char ALARM_MSG_4[] PROGMEM = "Corinthe";
const char ALARM_MSG_5[] PROGMEM = "Pasteur";
const char ALARM_MSG_6[] PROGMEM = "Clipper";
const char ALARM_MSG_7[] PROGMEM = "Le Regent";
const char ALARM_MSG_8[] PROGMEM = "Cap Esterel";
const char ALARM_MSG_9[] PROGMEM = "Espelido";
const char ALARM_MSG_10[] PROGMEM = "Parmiane";
const char* const ALARM_MSG_TAB[] PROGMEM = {ALARM_MSG_1,ALARM_MSG_2,ALARM_MSG_3,ALARM_MSG_4,ALARM_MSG_5,
                                             ALARM_MSG_6,ALARM_MSG_7,ALARM_MSG_8,ALARM_MSG_9,ALARM_MSG_10};

const char MENU_PARAMETRE[] PROGMEM = "Parametre";
  const char MENU_CONFIG[] PROGMEM = "Configuration";
  const char MENU_TMP_PAUSE[] PROGMEM ="Tmp de pause";
  const char MENU_TMP_DECAL[] PROGMEM = "Tmp decallage";
  const char MENU_TMP_PREAV[] PROGMEM = "Preavis";
  const char MENU_CMD_TYPE[] PROGMEM = "Cmd type";
    const char MENU_CMD_AUTO[] PROGMEM = "AUTO";
    const char MENU_CMD_REARM[] PROGMEM = "REARM";
const char MENU_MOTEUR[] PROGMEM = "Moteurs";
  const char MENU_GV[] PROGMEM = "Grande vitesse";
  const char MENU_PV[] PROGMEM = "Petite Vitesse";
  const char MENU_RALENT[] PROGMEM = "Ralentissement";
  const char MENU_M1_O_POS[] PROGMEM = "M1 Ouv Pos";
  const char MENU_M1_F_POS[] PROGMEM ="M1 Ferm Pos";
  const char MENU_M2_O_POS[] PROGMEM = "M2 Ouv Pos";
  const char MENU_M2_F_POS[] PROGMEM = "M2 Ferm Pos";
const char MENU_SECURITE[] PROGMEM = "Cellule";
  const char MENU_ACTIV_CELL[] PROGMEM = "Activer cell";
const char MENU_APPRENT[] PROGMEM = "Apprentissage";
const char MENU_SAUVEGARDE[] PROGMEM = "Sauvegarde";
  const char MENU_BOUTON_SAUVE[] PROGMEM = "<-ret    sauv->";
const char MENU_REVEIL[] PROGMEM = "Reveil";
const char MENU_REVEIL_HEURE[] PROGMEM = "Horloge";
const char MENU_REVEIL_REGLER_HEURE[] PROGMEM = "Regler Heure";
const char MENU_REVEIL_REGLER_ALARM[] PROGMEM = "Regler Alarme";
const char MENU_REVEIL_ACTIVER_ALARM[] PROGMEM = "Activer alarme";
const char MENU_CELL_MSG[] PROGMEM = "       C1";
const char MENU_APPRENT_BOUTON[] PROGMEM =  "+ Haut    Bas -";
const char MENU_TELECO[] PROGMEM = "Telecommande";
const char MENU_NOUV_TELECO[] PROGMEM = "Ajouter";
const char MENU_CLEAR_TELECO[] PROGMEM = "Effacer";

const char RUN_DYSPLAY_MODEL[] PROGMEM = "   Start-Ilex";
const char RUN_DYSPLAY_WAIT_CMD[] PROGMEM = "Att Cmd";
const char RUN_DYSPLAY_OPEN[] PROGMEM = "Ouverture";
const char RUN_DYSPLAY_CLOSE[] PROGMEM = "Fermeture";
const char RUN_DYSPLAY_PAUSE[] PROGMEM = "Pause";

void charCpy(char *src,char *dest);
void floatToStr(char *str, float x,unsigned int precision);

class MenuManager;
class MenuItem;
typedef MenuItem *(MenuManager::*paramFunctPtr)();

typedef struct
{
  bool up,down,left,right;
}ButtonInput;

class AlarmClock
{
  public:
    AlarmClock(RTC_DS3231 *rtc,unsigned long refreshTime);
    void getTime(uint8_t *hour,uint8_t *minute);
    void getTime(DateTime *time);
    void setTime(DateTime time) { _rtc->adjust(time);}
    float getYear(){return _temp;}
    void setAlarmHour(uint8_t hour) {_alarmHour=hour;}
    void setAlarmMinute(uint8_t minute) {_alarmMinute=minute;}
    uint8_t getAlarmHour() {return _alarmHour;}
    uint8_t getAlarmMinute() {return _alarmMinute;}
    float getTemperature() { return _temp;}
    bool alarmIsActivated() {return _alarmActivated;}
    void alarmActivate(bool activate) {_alarmActivated = activate;}
    bool alarmTrigger();

  private:
    StopWatch _refresh;
    RTC_DS3231 *_rtc;
    float _temp;
    DateTime _time;
    uint8_t _alarmHour,_alarmMinute;
    bool _alarmActivated;
};

class LcdDisplay
{
  public:
    LcdDisplay(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
    void setRow(const char *str,uint8_t row);
    void display();
    void clear() {_lcd.clear();}

  private:
    LiquidCrystal_I2C  _lcd;
    bool _refreshFlag;
    char _line[2][16];    
};

class MenuItem
{
  public:
    MenuItem(const char *name,ButtonInput *button);
    void setParent(MenuItem *parent) {_parent = parent;}
    void setLcd(LcdDisplay *lcd) { _lcd = lcd; }
    MenuItem *getParent() { return _parent; }
    virtual MenuItem *execute() = 0;
    char *getName() { return _name; }
  protected:
    MenuItem *_parent;
    ButtonInput *_button;
    char *_name;
    LcdDisplay *_lcd;
};

class SubMenu : public MenuItem
{
  public:
    SubMenu(const char *name ,ButtonInput *button);
    MenuItem *getChild(uint8_t childId);
    bool addItem(MenuItem *item);
    MenuItem *execute() override;

  protected:
    unsigned char _nbChild;
    unsigned char _cursorPos;
    MenuItem *_children[NB_CHILD_MAX];
};

class RootMenu : public SubMenu
{
  public:
    RootMenu(Gate *gatePtr ,ButtonInput *button,MenuManager* manager): SubMenu("ROOT",button),_gatePtr(gatePtr)
                                                            ,_paramReinit(false), _manager(manager),_rtc(NULL) {}
    MenuItem *execute() override; 
    void setRtcPtr(AlarmClock *rtc) {_rtc=rtc;} 
  protected:
    Gate *_gatePtr;
    bool _paramReinit;
    MenuManager *_manager;
    AlarmClock *_rtc;
};

class MenuAction : public MenuItem
{
    public:
      MenuAction(const char *name,paramFunctPtr function,MenuManager* manager
                                  ,ButtonInput *button): MenuItem(name,button), _function(function)
                                  ,_manager(manager) {}
      MenuItem *execute() override;
    private:
      paramFunctPtr _function;
      MenuManager* _manager;
};

class MenuManager
{
  public:
    MenuManager(Gate *gatePtr);
    void create();
    void display();
    void upInput() { _button.up = true;}
    void downInput() { _button.down = true;}
    void leftInput() { _button.left = true;}
    void validInput() { _button.right = true;}
    void clearButton();
    Gate *getGatePtr() { return _gatePtr;}
    void reinitParam() {_paramCpy = *_gatePtr->getParamPtr();}
    bool isRoot() { if(_curMenu == _rootMenu)return true;return false;}
    void setRtcPtr(RTC_DS3231 *rtc) {_rtcPtr = rtc;}
    void buzzerOn(bool activate);

  private:
    MenuItem *setPauseTime();
    MenuItem *setOffsetTime();
    MenuItem *setWarningTime();
    MenuItem *setCmdType();
    MenuItem *setHighSpeed();
    MenuItem *setLowSpeed();
    MenuItem *setSlowRatio();
    MenuItem *setM1OpenPos();
    MenuItem *setM1CLosePos();
    MenuItem *setM2OpenPos();
    MenuItem *setM2CLosePos();
    MenuItem *setSafe();
    MenuItem *saveParam();
    MenuItem *learning();
    MenuItem *clock();
    MenuItem *setClock();
    MenuItem *setAlarm();
    MenuItem *AlarmActivate();
    MenuItem *AlarmMsg();
    MenuItem *newRemote();

    RTC_DS3231 *_rtcPtr;
    AlarmClock *_rtc;
    MenuItem *_rootMenu,*_curMenu;
    Gate *_gatePtr;
    unsigned char _learningState;
    uint8_t _hourSetting, _minuteSetting,_idAlarmMsg;
    Blinker _buzzer;
    StopWatch _remoteClock;
    LcdDisplay *_lcd;
    ButtonInput _button;
    Param _paramCpy;
    
};



#endif // MENU_H_INCLUDED
