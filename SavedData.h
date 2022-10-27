
#ifndef SavedData_h
#define SavedData_h

#include "Arduino.h"

enum Person{
  CLAV = 1, // saved
  VLUC = 2, // saved
  PCO = 3, // saved
  JCHA = 4, // saved
  AMAA = 5, 
  NROL = 6, // saved
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