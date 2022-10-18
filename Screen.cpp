#include "Screen.h"


Screen::Screen(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6)
{
  _lcd = new LiquidCrystal(pin1, pin2, pin3, pin4, pin5, pin6);
}

void Screen::setup()
{
  _lcd->begin(16, 2);
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