#include "Comparison.h"

UserProfile Comparison::findOppositeRiskProfile(UserProfile& currentUser, std::vector<UserProfile>& data, StrokeRiskPredictor& predictor) {
    std::string currentRisk = predictor.generateAlert(predictor.calculateRisk(currentUser));
    for (auto& u : data) {
        std::string otherRisk = predictor.generateAlert(predictor.calculateRisk(u));
        if (otherRisk != currentRisk) return u;
    }
    return UserProfile();
}