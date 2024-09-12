#include "SavedData.h"

#include <EEPROM.h>

const bool enable_EEPROM = true;

SavedData::SavedData(){}

void SavedData::setup(){
  for(int personne = 0; personne < UNKOWN; personne ++)
  {
    if(enable_EEPROM)
      scores[personne] = EEPROM.read(personne);

    Serial.println(toString(personne));
    Serial.println(scores[personne]);
  }
}

void SavedData::reset(){
  Serial.println("RESET");
  
  for(int personne = CLAV; personne < UNKOWN; personne ++)
  {
    scores[personne] = 0;
    if(enable_EEPROM)
      EEPROM.write(personne, 0);
  }

  EEPROM.write(0, 0);
}

int SavedData::getScore(Person person){
  if(person >= UNKOWN || person < CLAV)
  {
    return -1;
  }
  
  return scores[person];
}

void SavedData::incrScore(Person person)
{
  if(person >= UNKOWN || person < CLAV){
    Serial.println("incrScore error, person is out of range");
    Serial.println(person);
    return;
  }

  scores[person] = scores[person] + 1;

  if(enable_EEPROM)
    EEPROM.write((int)person, scores[person]);
}

String SavedData::toString(Person person){
  switch(person){
    case NROL:
      return "Nicolas";

    case CLAV:
      return "Clement";

    case VLUC:
      return "Valentin";

    case PCO:
      return "Pierre";

    case JCHA:
      return "Jean.C";

    case AMAA:
      return "Alexis";

    case JDUV:
      return "Jean.D";

    case UNKOWN:
    default:
      return "Inconnu";
  }

  return "Personne non trouvée";
}

void SavedData::setScore(Person person, int score)
{
  if(person >= UNKOWN || person < CLAV){
    Serial.println("setScore error, person is out of range");
    Serial.println(person);
    return;
  }

  scores[person] = score;

  if(enable_EEPROM)
    EEPROM.write((int)person, scores[person]);
}
