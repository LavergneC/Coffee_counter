#include "Fingerprint.h"

#define LOOP_DELAY 50 // ms

const bool bypass = false;

Fingerprint::Fingerprint(){}

bool Fingerprint::setup(uint8_t pinRx, uint8_t pinTx){
  if(bypass)
    return true;

  Serial.println("\n\nBuilding Fingerprint");
  delay(100);
  _mySerial = new SoftwareSerial(pinRx, pinTx);
  
  Serial.println("\n\nSerial creation...");
   delay(100);
  _finger = new Adafruit_Fingerprint(_mySerial);
  
  Serial.println("\n\nbuilds OKs");

  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  _finger->begin(57600);
  delay(5);
  if (_finger->verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor");
    return false;
  }

  Serial.println(F("Reading sensor parameters"));
  _finger->getParameters();
  Serial.print(F("Status: 0x")); Serial.println(_finger->status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(_finger->system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(_finger->capacity);
  Serial.print(F("Security level: ")); Serial.println(_finger->security_level);
  Serial.print(F("Device address: ")); Serial.println(_finger->device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(_finger->packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(_finger->baud_rate);

  _finger->getTemplateCount();

  if (_finger->templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains "); Serial.print(_finger->templateCount); Serial.println(" templates");
  }

  return true;
}

void Fingerprint::loop(){
  if(bypass)
    return true;
    
  static unsigned long lastExec = millis();

  if(millis() - lastExec < LOOP_DELAY)
    return; 

  _finger_read = getFingerprintData(); // also update _last_person

  lastExec = millis();
}

bool Fingerprint::read(){
  if(bypass)
    return false;

  bool ret = _finger_read;
  _finger_read = false;
  
  return ret;
}

// Return value : false -> no  finger detected
//                true -> Finger detected (may be unknow)
bool Fingerprint::getFingerprintData(){
  if(bypass)
    return false;

  uint8_t p = _finger->getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      //Serial.println("No finger detected");
      return false;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return false;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return false;
    default:
      Serial.println("Unknown error");
      return false;
  }

  // OK success!

  p = _finger->image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return false;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return false;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return false;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return false;
    default:
      Serial.println("Unknown error");
      return false;
  }

  // OK converted!
  p = _finger->fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return false;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    _last_person = UNKOWN;
    return true;
  } else {
    Serial.println("Unknown error");
    return false;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(_finger->fingerID);
  Serial.print(" with confidence of "); Serial.println(_finger->confidence);

  _last_person = _finger->fingerID;
  return true;
}

bool Fingerprint::getFingerprintData2(){
  if(bypass)
    return true;
    
  uint8_t p = _finger->getImage();
  if(p != FINGERPRINT_OK){
    return false;
  }

  p = _finger->image2Tz();
  if(p != FINGERPRINT_OK){
    return false;
  }

  p = _finger->fingerSearch();
  if(p == FINGERPRINT_NOTFOUND){
    _last_person = UNKOWN;
    return true;
  }

  if (p != FINGERPRINT_OK) {
    return false;
  }

  // Found someone
  _last_person = _finger->fingerID;
  return true;  
}
