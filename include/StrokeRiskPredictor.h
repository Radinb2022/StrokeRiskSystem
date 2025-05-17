#ifndef STROKE_RISK_PREDICTOR_H
#define STROKE_RISK_PREDICTOR_H

#include "UserProfile.h"
#include "KaggleData.h"
#include "RandomForest.h"
#include <string>

class StrokeRiskPredictor {
public:
    RandomForest forest;
    void train(const std::vector<UserProfile>& data);
    int predict(const UserProfile& user) const;
    float predict_proba(const UserProfile& user) const;
    // Old rule-based (fallback)
    float calculateRisk(UserProfile& user);
    std::string generateAlert(float risk);
    void addTrainingData(UserProfile& user, KaggleData& data);
};

#endif