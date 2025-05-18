#ifndef COMPARISON_ENGINE_H
#define COMPARISON_ENGINE_H

#include "UserProfile.h"
#include "DataProcessor.h"
#include "RandomForest.h"
#include <vector>
#include <string>
#include <limits>

class ComparisonEngine {
public:
    ComparisonEngine();

    UserProfile findSimilarOppositeRisk(const UserProfile& userProfile,
                                      const DataProcessor& dataProcessor,
                                      const RandomForest& model) const;

private:
    double calculateSimilarity(const UserProfile& profile1, const UserProfile& profile2,
                               const DataProcessor& processor) const;

    std::string getRiskString(int risk) const;
};

#endif // COMPARISON_ENGINE_H