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
Screen screen(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6, LCD_LED);
SavedData savedData;
Fingerprint fingerprint;
Bouton boutonReset(BTN_RESET, RST_DELAY);
MyMusique musique(BIPPER);
Person lastPerson = UNKOWN + 1;

void setup(){
  Serial.begin(9600);
  savedData.setup();
  screen.setup(&savedData);

  bool sucess = fingerprint.setup(A1, A0);
  if(!sucess){
    screen.printFingerError();
    while(1){delay(100);}
  }

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void sucessSequence(Person person){
  savedData.incrScore(person);
  //screen.printCoffees(savedData.toString(person), savedData.getScore(person), true);
  screen.jumpTo(person);
  
  digitalWrite(LED_GREEN, true);
  musique.playSucess(person + 1);
  delay(PRINT_DELAY);
  
  digitalWrite(LED_GREEN, false);
}

void errorSequence(){
  screen.printWrongFinger();
  
  digitalWrite(LED_RED, true);
  musique.playError();
  delay(ERROR_DELAY);

  digitalWrite(LED_RED, false);
}

void loop() {
  fingerprint.loop();
  boutonReset.loop();
  screen.loop();

  digitalWrite(LED_YELLOW, digitalRead(BTN_RESET)); // yellow LED matchs RST button

  if(fingerprint.read())
  {
    Person readFinger = fingerprint.last_person_read();
        
    if(readFinger != UNKOWN) {
      sucessSequence(readFinger);
    } else {
      errorSequence();
    }

    screen.forceUpdateScreen();
  }

  if(boutonReset.pressed())
  {
    savedData.reset();
    musique.playReset();
  }

  if(digitalRead(BTN_RESET)){ // Change with a new buton using the dedicated class
    screen.wakeup();
  }
}
