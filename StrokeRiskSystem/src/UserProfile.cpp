#include "UserProfile.h"

UserProfile::UserProfile()
    : age(0), highBloodPressure(false), chestPain(false), dizziness(false),
      fatigueWeakness(false), swellingEdema(false), painNeckJawShoulderBack(false),
      excessiveSweating(false), chestDiscomfortActivity(false),
      snoringSleepApnea(false), anxietyFeelingDoom(false),
      strokePrediction(-1) {}

void UserProfile::setAge(int age) { this->age = age; }
void UserProfile::setHighBloodPressure(bool value) { this->highBloodPressure = value; }
void UserProfile::setChestPain(bool value) { this->chestPain = value; }
void UserProfile::setDizziness(bool value) { this->dizziness = value; }
void UserProfile::setFatigueWeakness(bool value) { this->fatigueWeakness = value; }
void UserProfile::setSwellingEdema(bool value) { this->swellingEdema = value; }
void UserProfile::setPainNeckJawShoulderBack(bool value) { this->painNeckJawShoulderBack = value; }
void UserProfile::setExcessiveSweating(bool value) { this->excessiveSweating = value; }
void UserProfile::setChestDiscomfortActivity(bool value) { this->chestDiscomfortActivity = value; }
void UserProfile::setSnoringSleepApnea(bool value) { this->snoringSleepApnea = value; }
void UserProfile::setAnxietyFeelingDoom(bool value) { this->anxietyFeelingDoom = value; }
void UserProfile::setStrokePrediction(int prediction) { this->strokePrediction = prediction; }

int UserProfile::getAge() const { return age; }
bool UserProfile::getHighBloodPressure() const { return highBloodPressure; }
bool UserProfile::getChestPain() const { return chestPain; }
bool UserProfile::getDizziness() const { return dizziness; }
bool UserProfile::getFatigueWeakness() const { return fatigueWeakness; }
bool UserProfile::getSwellingEdema() const { return swellingEdema; }
bool UserProfile::getPainNeckJawShoulderBack() const { return painNeckJawShoulderBack; }
bool UserProfile::getExcessiveSweating() const { return excessiveSweating; }
bool UserProfile::getChestDiscomfortActivity() const { return chestDiscomfortActivity; }
bool UserProfile::getSnoringSleepApnea() const { return snoringSleepApnea; }
bool UserProfile::getAnxietyFeelingDoom() const { return anxietyFeelingDoom; }
int UserProfile::getStrokePrediction() const { return strokePrediction; }