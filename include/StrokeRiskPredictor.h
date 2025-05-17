#ifndef STROKERISKPREDICTOR_H
#define STROKERISKPREDICTOR_H

#include "UserProfile.h"
#include "RFClassifier.h"
#include "KaggleData.h"
#include <string>

class StrokeRiskPredictor {
private:
    RFClassifier forest;

public:
    void TrainModel(const std::vector<UserProfile>& data);
    int PredictRisk(const UserProfile& user) const;
    double RiskProbability(const UserProfile& user) const;
    std::string OutcomeMessage(const UserProfile& user) const;
    
    // Rule-based fallback methods
    float calculateRisk(UserProfile& user);
    std::string generateAlert(float risk);
    void addTrainingData(UserProfile& user, KaggleData& data);
};

#endif