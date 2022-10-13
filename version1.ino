#include <LiquidCrystal.h>
#include "Arduino.h"

#include "Bouton.h"
#include "SavedData.h"
#include "Fingerprint.h"

#define LED_RED A4
#define LED_YELLOW 13
#define LED_GREEN 6

#define BTN_FINGER 7
#define BTN_RESET 8
#define BTN_2 6
#define BTN_3 7
#define BIPPER 9

#define POTAR A5
#define MAX_ANALOG_VALUE 1000

LiquidCrystal lcd(12,11,5,4,A2,A3);
SavedData savedData;
Fingerprint fingerprint;
RebBouton boutonFinger(BTN_FINGER);
RebBouton boutonReset(BTN_RESET, 3500);

int notes[] = {131, 165, 191};

void highPrint(Person person){
  lcd.clear();
  lcd.setCursor(0,0);

  if(person >= UNKOWN || person < 0){
    lcd.print("Input error");
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

void setup() {
  lcd.begin(16, 2);

  Serial.begin(9600);
  savedData.setup();
  fingerprint.setup(2, 3);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(POTAR, INPUT);
}

Person getPerson()
{
  int potartValue = analogRead(POTAR);
  Person p = potartValue / (MAX_ANALOG_VALUE / (UNKOWN + 1));
  if(p > UNKOWN)
    p = UNKOWN;

  return p; 
}

Person lastPerson = UNKOWN + 1;

void loop() {
  boutonFinger.loop();
  boutonReset.loop();
  digitalWrite(LED_YELLOW, digitalRead(BTN_RESET));

  Person printPerson = getPerson();
  
  if(lastPerson != printPerson)
    highPrint(printPerson);
  
  lastPerson = printPerson;

  Person selected = fingerprint.getFingerprintIDez();
  
  if(selected != -1)
  {
    if(selected == UNKOWN){
      digitalWrite(LED_RED, true);
      tone(BIPPER, notes[0], 100);
      delay(170);
      tone(BIPPER, notes[0], 300);
      delay(300);

      highPrint(selected);
      
      digitalWrite(LED_RED, false);
      return;
    }
    
    digitalWrite(LED_GREEN, true);
    savedData.incrScore(selected);
    highPrint(selected);
    
    tone(BIPPER, notes[0] * pow(1.06, (selected + 1)  * 4), 100);
    delay(200);
    tone(BIPPER, notes[1] * pow(1.06, (selected + 1)  * 4), 100);
    delay(200);
    tone(BIPPER, notes[2] * pow(1.06, (selected + 1)  * 4), 100);
    delay(100);
    
    digitalWrite(LED_GREEN, false);
    selected = UNKOWN; // to update next loop
  }

  if(boutonReset.pressed())
  {
    tone(BIPPER, notes[2], 500);
    delay(500);
    tone(BIPPER, notes[0], 500);
    delay(500);
    
    savedData.reset();
    selected = UNKOWN; // to update next loop
  }
}
