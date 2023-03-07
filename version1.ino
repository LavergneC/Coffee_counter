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

#define WIP 0
#define VERSION {1, 0, 0, WIP} // major, minor, patch, wip(bool)

// Variables initialization
Screen screen(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6, LCD_LED);
SavedData savedData;
Fingerprint fingerprint;
Bouton boutonReset(BTN_RESET, RST_DELAY);
MyMusique musique(BIPPER);
Person lastPerson = UNKOWN + 1;

void setup(){
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  digitalWrite(LED_RED, true);
  
  Serial.begin(9600);
  savedData.setup();

  int version[4] = VERSION;
  screen.setup(&savedData, version);

  bool sucess = fingerprint.setup(FINGER_RX, FINGER_TX);
  if(!sucess){
    screen.printFingerError();
    while(1){delay(100);}
  }

  digitalWrite(LED_RED, false);
  musique.playBoot();
  delay(1000);
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
    screen.forceUpdateScreen();
    Person readFinger = fingerprint.last_person_read();
        
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

  if(digitalRead(BTN_RESET)){ // Change with a new buton using the dedicated class
    screen.wakeup();
  }
}
