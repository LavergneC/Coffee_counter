#include "Arduino.h"
#include "Screen.h"

#define PERIOD_AUTO_CHANGE 3000
#define PERIOD_BACKLIGHT UNKOWN * (PERIOD_AUTO_CHANGE)

Screen::Screen(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int led_pin) : _led_pin(led_pin)
{
  _lcd = new LiquidCrystal(pin1, pin2, pin3, pin4, pin5, pin6);
}

void Screen::setup(SavedData* savedData, int version[4])
{
  _lcd->begin(16, 2);
  _savedData = savedData;

  pinMode(_led_pin, OUTPUT);
  digitalWrite(_led_pin, true);
  _sleepTime = millis();

  printVersion(version);
}

void Screen::printVersion(int version[4]){
  _lcd->print("Version");
  _lcd->setCursor(0, 1);
  _lcd->print(version[0]);
  _lcd->print(".");
  _lcd->print(version[1]);
  _lcd->print(".");
  _lcd->print(version[2]);
  if(version[3])
    _lcd->print(" WIP");
}

void Screen::loop(){
  static unsigned long lastAutoUpdate = millis();
  
  if(_sleep){
    return;
  }

  if(_updateScreen)
  {
    updateScreen();
    lastAutoUpdate = millis();
    return;
  }

  if(millis() - _sleepTime > PERIOD_BACKLIGHT){
    clear();
    digitalWrite(_led_pin, false);
    _sleep = true;
  }

  if(millis() - lastAutoUpdate > PERIOD_AUTO_CHANGE){
    _personToPrint = _personToPrint + 1;

    if(_personToPrint >= UNKOWN)
    {
      _personToPrint = CLAV;
    }

    _updateScreen = true;
  }
}

void Screen::jumpTo(Person person)
{
  _personToPrint = person;
  updateScreen();
  forceUpdateScreen();
}

void Screen::wakeup()
{
  _sleepTime = millis();

  if(!_sleep)
    return;

  digitalWrite(_led_pin, true);
  _updateScreen = true;
  _sleep = false;
}

void Screen::forceUpdateScreen(){
  wakeup();
  _updateScreen = true;
}

void Screen::updateScreen(){
  printCoffees(_savedData->toString(_personToPrint), _savedData->getScore(_personToPrint));
  _updateScreen = false;
}

void Screen::printCoffees(String nom, int score){
  clear();

  if(score == -1){
    _lcd->print("Input error");
    return;    
  }

  _lcd->print(nom);
  _lcd->setCursor(0, 1);
  _lcd->print(score);
  _lcd->print(" cafe");
  
  if(score > 1)
    _lcd->print("s");
}

void Screen::printFingerError(){
  clear();
  _lcd->print("ERR: Finger init");
  _lcd->setCursor(0,1);
  _lcd->print("Need reset");
}

void Screen::clear(){
  _lcd->clear();
  _lcd->setCursor(0,0);
}

void Screen::printWrongFinger(){
  clear();
  _lcd->print("ERR: Finger not");
  _lcd->setCursor(0,1);
  _lcd->print("recognized");
}