#include "Arduino.h"

#include "Bouton.h"
#include "SavedData.h"
#include "Fingerprint.h"
#include "Musique.h"
#include "PinOut.h"
#include "Screen.h"

#define MAX_ANALOG_VALUE 1000
#define RST_DELAY 3000
#define PRINT_DELAY 2000
#define ERROR_DELAY 300

// Variables initialization
Screen screen(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6);
SavedData savedData;
Fingerprint fingerprint;
Bouton boutonReset(BTN_RESET, RST_DELAY);
MyMusique musique(BIPPER);
Person lastPerson = UNKOWN + 1;
bool updateScreen = false;

void setup(){
  Serial.begin(9600);
  savedData.setup();
  screen.setup();

  bool sucess = fingerprint.setup(2, 3);
  if(!sucess){
    screen.printFingerError();
    while(1){delay(100);}
  }

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
  screen.printCoffees(savedData.toString(person), savedData.getScore(person));
  musique.playSucess(person + 1);
  delay(PRINT_DELAY);
  
  digitalWrite(LED_GREEN, false);
}

void errorSequence(){
  digitalWrite(LED_RED, true);

  screen.printWrongFinger();
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
    screen.printCoffees(savedData.toString(printPerson), savedData.getScore(printPerson));
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
    updateScreen = true;
  }
}
