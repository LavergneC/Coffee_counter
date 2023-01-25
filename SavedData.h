
#ifndef SavedData_h
#define SavedData_h

#include "Arduino.h"

enum Person{
  CLAV = 1, 
  VLUC = 2,
  PCO = 3, 
  JCHA = 4, 
  AMAA = 5,
  NROL = 6,
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
  void setScore(Person person, int score);
  void reset();
  String toString(Person person);
};

#endif