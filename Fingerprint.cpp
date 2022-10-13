#include "Fingerprint.h"

Fingerprint::Fingerprint(){}

void Fingerprint::setup(uint8_t pinRx, uint8_t pinTx){
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
    Serial.println("Did not find fingerprint sensor: lock loop.");
    while (1) { delay(1); }
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
}

void Fingerprint::loop(){
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.*/
}

uint8_t Fingerprint::getFingerprintID(){
  uint8_t p = _finger->getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = _finger->image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = _finger->fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(_finger->fingerID);
  Serial.print(" with confidence of "); Serial.println(_finger->confidence);

  return _finger->fingerID;
}

int Fingerprint::getFingerprintIDez(){
  uint8_t p = _finger->getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = _finger->image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = _finger->fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(_finger->fingerID);
  Serial.print(" with confidence of "); Serial.println(_finger->confidence);
  return _finger->fingerID;
}
