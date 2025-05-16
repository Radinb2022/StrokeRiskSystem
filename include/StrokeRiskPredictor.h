#ifndef STROKE_RISK_PREDICTOR_H
#define STROKE_RISK_PREDICTOR_H

#include "UserProfile.h"
#include <string>

class StrokeRiskPredictor {
public:
    float calculateRisk(UserProfile& user);
    std::string generateAlert(float risk);
};

#endif