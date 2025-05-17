#include "../include/StrokeRiskPredictor.h"
#include <algorithm>

void StrokeRiskPredictor::TrainModel(const std::vector<UserProfile>& data) {
    forest.Train(data);
}

int StrokeRiskPredictor::PredictRisk(const UserProfile& user) const {
    return forest.Predict(user);
}

double StrokeRiskPredictor::RiskProbability(const UserProfile& user) const {
    return forest.PredictProba(user);
}

std::string StrokeRiskPredictor::OutcomeMessage(const UserProfile& user) const {
    double p = RiskProbability(user);
    int cls = PredictRisk(user);
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "Prediction: %s (%.2f%% probability)",
             cls ? "At Risk" : "Not At Risk", p * 100.0);
    return std::string(buffer);
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
    TrainModel(data.dataset);
}