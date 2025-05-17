#ifndef COMPARISON_H
#define COMPARISON_H

#include "UserProfile.h"
#include "StrokeRiskPredictor.h"
#include <vector>

class Comparison {
public:
    UserProfile findOppositeRiskProfile(UserProfile& currentUser, std::vector<UserProfile>& data, StrokeRiskPredictor& predictor);
};

#endif
