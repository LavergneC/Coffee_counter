#ifndef SCREEN_H
#define SCREEN_H

#include <LiquidCrystal.h>
#include "Arduino.h"
#include "SavedData.h"


class Screen {

private:
  LiquidCrystal* _lcd;
  Person _personToPrint = CLAV;
  bool _updateScreen = true;
  bool _sleep = false;
  unsigned long _sleepTime;
  SavedData* _savedData;
  int _led_pin;

public:
  Screen(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int led_pin);
  void setup(SavedData* savedData);

  void loop();
  void updateScreen();
  void forceUpdateScreen();
  void wakeup();

  void printCoffees(String nom, int score);
  void printFingerError();
  void printWrongFinger();
  void jumpTo(Person person);

  void clear();
};


#endif