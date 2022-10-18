#ifndef SCREEN_H
#define SCREEN_H

#include <LiquidCrystal.h>
#include "Arduino.h"


class Screen {

private:
  LiquidCrystal* _lcd;

public:
  Screen(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6);
  void setup();

  void printCoffees(String nom, int score);
  void printFingerError();
  void printWrongFinger();

  void clear();
};


#endif