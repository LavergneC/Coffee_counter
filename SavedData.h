
#ifndef SavedData_h
#define SavedData_h

#include "Arduino.h"

enum Person{
  NROL = 0,
  CLAV,
  VLUC,
  PCO,
  JCHA,
  AMAA,
  UNKOWN //last
};

class SavedData
{
private:
  int scores[UNKOWN]= {0};

public:
  SavedData();

  void setup();
  int getScore(Person person);
  void incrScore(Person person);
  void reset();
  String toString(Person person);
};

#endif