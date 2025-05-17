#include "StrokeRiskPredictor.h"
#include <algorithm>

void StrokeRiskPredictor::train(const std::vector<UserProfile>& data) {
    forest.train(data);
}

int StrokeRiskPredictor::predict(const UserProfile& user) const {
    return forest.predict(user);
}

float StrokeRiskPredictor::predict_proba(const UserProfile& user) const {
    return forest.predict_proba(user);
}

float StrokeRiskPredictor::calculateRisk(UserProfile& user) {
    float score = 0;
    if (user.age >= 70) score += 30;
    if (user.healthIndicators["HighBloodPressure"]) score += 25;
    if (user.healthIndicators["ChestPain"]) score += 20;
    if (user.healthIndicators["Dizziness"]) score += 10;
    return std::min(score, 100.0f);
}

std::string StrokeRiskPredictor::generateAlert(float risk) {
    return (risk >= 50) ? "At Risk" : "Not At Risk";
}

void StrokeRiskPredictor::addTrainingData(UserProfile& user, KaggleData& data) {
    data.appendUser(user);
    train(data.dataset);
}