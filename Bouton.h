
#ifndef Boutons_h
#define Boutons_h

#include "Arduino.h"

class RebBouton
{
private:
  const unsigned long m_debounceDelay;
  const int m_didPin;

  unsigned long m_lastDebounceTime = 0;

  bool m_didStatusRead = false;
  bool m_buttonPressed = false;
  bool m_didStatus = false;
  bool m_oldDidStatus = false;

public:
  RebBouton(int pin, int delay = 50);
  
  void loop();
  bool pressed();
};

#endif