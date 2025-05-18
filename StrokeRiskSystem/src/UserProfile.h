#ifndef USER_PROFILE_H
#define USER_PROFILE_H

#include <string>
#include <vector>
#include <map>

class UserProfile {
public:
    UserProfile();

    // Setters
    void setAge(int age);
    void setHighBloodPressure(bool value);
    void setChestPain(bool value);
    void setDizziness(bool value);
    void setFatigueWeakness(bool value);
    void setSwellingEdema(bool value);
    void setPainNeckJawShoulderBack(bool value);
    void setExcessiveSweating(bool value);
    void setChestDiscomfortActivity(bool value);
    void setSnoringSleepApnea(bool value);
    void setAnxietyFeelingDoom(bool value);
    void setStrokePrediction(int prediction); // 0 or 1 for Not At Risk / At Risk

    // Getters
    int getAge() const;
    bool getHighBloodPressure() const;
    bool getChestPain() const;
    bool getDizziness() const;
    bool getFatigueWeakness() const;
    bool getSwellingEdema() const;
    bool getPainNeckJawShoulderBack() const;
    bool getExcessiveSweating() const;
    bool getChestDiscomfortActivity() const;
    bool getSnoringSleepApnea() const;
    bool getAnxietyFeelingDoom() const;
    int getStrokePrediction() const;

    // A dummy for 'gender' as it was used in previous code for UserProfile default constructor
    // but now removed. Will be removed from DataProcessor too but needed for minimal changes.
    std::string getGender() const { return ""; } // Dummy getter, not used for new features

private:
    int age;
    bool highBloodPressure;
    bool chestPain;
    bool dizziness;
    bool fatigueWeakness;
    bool swellingEdema;
    bool painNeckJawShoulderBack;
    bool excessiveSweating;
    bool chestDiscomfortActivity;
    bool snoringSleepApnea;
    bool anxietyFeelingDoom;
    int strokePrediction; // -1: unknown, 0: Not At Risk, 1: At Risk
};

#endif // USER_PROFILE_H