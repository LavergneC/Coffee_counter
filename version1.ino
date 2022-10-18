#include <LiquidCrystal.h>
#include "Arduino.h"

#include "Bouton.h"
#include "SavedData.h"
#include "Fingerprint.h"
#include "Musique.h"
#include "PinOut.h"

#define MAX_ANALOG_VALUE 1000
#define RST_DELAY 3000
#define PRINT_DELAY 2000
#define ERROR_DELAY 300

// Variables initialization
LiquidCrystal lcd(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6);
SavedData savedData;
Fingerprint fingerprint;
Bouton boutonReset(BTN_RESET, RST_DELAY);
MyMusique musique(BIPPER);
Person lastPerson = UNKOWN + 1;
bool updateScreen = false;

void highPrint(Person person){
  lcd.clear();
  lcd.setCursor(0,0);

  if(person > UNKOWN || person < 0){
    lcd.print("Input error");
    return;
  }
  
  if(person == UNKOWN){
    lcd.print("Doigt non");
    lcd.setCursor(0, 1);
    lcd.print("reconnu");
    return;
  }

  String nom = savedData.toString(person);
  int score = savedData.getScore(person);

  lcd.print(nom);
  lcd.setCursor(0, 1);
  lcd.print(score);
  lcd.print(" cafe");
  
  if(score > 1)
    lcd.print("s");
}

void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  savedData.setup();
  fingerprint.setup(2, 3);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(POTAR, INPUT);
}

Person getPersonPotard()
{
  int potartValue = analogRead(POTAR);
  Person p = (potartValue / (MAX_ANALOG_VALUE / (UNKOWN + 1))) + 1;
  if(p > UNKOWN || p == 0)
    p = UNKOWN;

  return p; 
}

void sucessSequence(Person person){
  digitalWrite(LED_GREEN, true);

  savedData.incrScore(person);
  highPrint(person);
  musique.playSucess(person + 1);
  delay(PRINT_DELAY);
  
  digitalWrite(LED_GREEN, false);
}

void errorSequence(){
  digitalWrite(LED_RED, true);

  highPrint(UNKOWN);
  musique.playError();
  delay(ERROR_DELAY);

  digitalWrite(LED_RED, false);
}

void loop() {
  fingerprint.loop();
  boutonReset.loop();
  digitalWrite(LED_YELLOW, digitalRead(BTN_RESET)); // yellow LED matchs RST button

  // continuous print block
  Person printPerson = getPersonPotard();
  if(lastPerson != printPerson || updateScreen){
    highPrint(printPerson);
    updateScreen = false;
  }
  lastPerson = printPerson;
  // ---

  if(fingerprint.read())
  {
    Person readFinger = fingerprint.last_person_read();
    updateScreen = true;
    
    if(readFinger != UNKOWN) {
      sucessSequence(readFinger);
    } else {
      errorSequence();
    }
  }

  if(boutonReset.pressed())
  {
    savedData.reset();
    musique.playReset();
  }
}
