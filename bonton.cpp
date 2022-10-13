
#include "Bouton.h"

RebBouton::RebBouton(int pin, int delay): m_didPin(pin), m_debounceDelay(delay)
{
  pinMode(pin, INPUT);
}

bool RebBouton::pressed(){
  bool ret = m_buttonPressed;

  if(m_buttonPressed && !m_didStatusRead )
    m_didStatusRead = true;

  return ret;
}

void RebBouton::loop() {
  int reading = digitalRead(m_didPin);
  if (reading != m_oldDidStatus) {
    m_lastDebounceTime = millis();
  }
  if ((millis() - m_lastDebounceTime)>= m_debounceDelay) {
    if (reading != m_didStatus) {
      m_didStatus = reading;
      //Serial.print(F("Sensor state : ")); Serial.println(m_didStatus);
    }
  }
  m_oldDidStatus = reading;

  if(m_didStatusRead && m_didStatus == 0){
    m_didStatusRead = false;
  }

  m_buttonPressed = !m_didStatusRead && m_didStatus;
}
