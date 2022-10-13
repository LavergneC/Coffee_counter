#ifndef FingerPrint_h
#define FingerPrint_h

#include <Adafruit_Fingerprint.h>
#include "SavedData.h"

class Fingerprint
{
private:
  SoftwareSerial* _mySerial;
  Adafruit_Fingerprint* _finger;
  
public:
  Fingerprint();

  void setup(uint8_t pinRx, uint8_t pinTx);
  void loop();
  uint8_t getFingerprintID();
  int getFingerprintIDez();
};

#endif