#ifndef Musique_h
#define Musique_h

#include "Arduino.h"

class MyMusique
{
private:
  int _pin;
  const int _notes[3] = {131, 165, 191};

  void playNote(int note_idx, int note_high, int note_duration);
  void playNote(int note_idx, int note_duration);

public:
  MyMusique(int pin);

  void playSucess(int high);
  void playError();
  void playReset();
};

#endif