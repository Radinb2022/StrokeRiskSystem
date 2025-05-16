#include "StrokeRiskPredictor.h"
#include <algorithm>

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