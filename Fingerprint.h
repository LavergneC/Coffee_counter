#ifndef FingerPrint_h
#define FingerPrint_h

#include <Adafruit_Fingerprint.h>
#include "SavedData.h"

class Fingerprint
{
private:
  HardwareSerial* _mySerial;
  Adafruit_Fingerprint* _finger;
  
  bool _finger_read = false;
  Person _last_person = UNKOWN;
  
  bool getFingerprintData();
  bool getFingerprintData2();
  
public:
  Fingerprint();

  bool setup(uint8_t pinRx, uint8_t pinTx);
  void loop();

  bool read();
  Person last_person_read(){return _last_person;}
};

#endif