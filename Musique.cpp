#include "Musique.h"

MyMusique::MyMusique(int pin) : _pin(pin) {}

void MyMusique::playNote(int note_idx, int note_high, int note_duration){
  int note = _notes[note_idx] * pow(1.06, note_high  * 4);

  tone(_pin, note, note_duration);
}

void MyMusique::playNote(int note_idx, int note_duration)
{
  tone(_pin, _notes[note_idx], note_duration);
}

void MyMusique::playSucess(int high)
{
  playNote(0, high, 100);
  delay(200);
  
  playNote(1, high, 100);
  delay(200);

  playNote(2, high, 100);
  delay(100);
}

void MyMusique::playError()
{
  playNote(0, 300);
  delay(300);  
}

void MyMusique::playReset()
{
  playNote(2, 500);
  delay(500);
  playNote(0, 500);
  delay(500);
}