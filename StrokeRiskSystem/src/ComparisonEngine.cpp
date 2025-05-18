#include "ComparisonEngine.h"
#include <cmath>
#include <iostream>

ComparisonEngine::ComparisonEngine() {
}

UserProfile ComparisonEngine::findSimilarOppositeRisk(const UserProfile& userProfile,
                                                  const DataProcessor& dataProcessor,
                                                  const RandomForest& model) const {
    UserProfile bestMatchProfile;
    double minDistance = std::numeric_limits<double>::max();

    int userRisk = userProfile.getStrokePrediction();
    if (userRisk == -1) {
        std::cerr << "ComparisonEngine: User's risk status is unknown. Cannot find opposite." << std::endl;
        return UserProfile();
    }
    int targetOppositeRisk = (userRisk == 0) ? 1 : 0;

    const std::vector<UserProfile>& allData = dataProcessor.getRawData();

    if (allData.empty()) {
        std::cerr << "ComparisonEngine: Dataset is empty, cannot find similar profile." << std::endl;
        return UserProfile();
    }

    for (const auto& dataProfile : allData) {
        // Simple check for default-constructed profile (e.g., if parsing failed for a row)
        if (dataProfile.getAge() == 0 && !dataProfile.getHighBloodPressure() && !dataProfile.getChestPain()) {
            continue;
        }

        std::string dataProfileRiskStr = model.predict(dataProcessor.preprocessUserProfile(dataProfile));
        int dataProfileRisk = (dataProfileRiskStr == "1") ? 1 : 0;

        if (dataProfileRisk == targetOppositeRisk) {
            double distance = calculateSimilarity(userProfile, dataProfile, dataProcessor);
            if (distance < minDistance) {
                minDistance = distance;
                bestMatchProfile = dataProfile;
                bestMatchProfile.setStrokePrediction(dataProfileRisk);
            }
        }
    }
    std::cout << "ComparisonEngine: Best match found with distance: " << minDistance << std::endl;
    return bestMatchProfile;
}

double ComparisonEngine::calculateSimilarity(const UserProfile& profile1, const UserProfile& profile2,
                                         const DataProcessor& processor) const {
    std::vector<double> features1 = processor.preprocessUserProfile(profile1);
    std::vector<double> features2 = processor.preprocessUserProfile(profile2);

    if (features1.empty() || features2.empty() || features1.size() != features2.size()) {
        std::cerr << "ComparisonEngine::calculateSimilarity: Feature vectors are invalid or mismatched." << std::endl;
        return std::numeric_limits<double>::max();
    }

    double sum_sq_diff = 0.0;
    for (size_t i = 0; i < features1.size(); ++i) {
        sum_sq_diff += std::pow(features1[i] - features2[i], 2);
    }
    return std::sqrt(sum_sq_diff);
}

std::string ComparisonEngine::getRiskString(int risk) const {
    if (risk == 0) return "Not At Risk";
    if (risk == 1) return "At Risk";
    return "Unknown";
}