#ifndef STROKE_RISK_PREDICTOR_H
#define STROKE_RISK_PREDICTOR_H

#include "RandomForest.h"
#include "UserProfile.h"
#include "DataProcessor.h"

class StrokeRiskPredictor {
public:
    StrokeRiskPredictor(RandomForest* model);

    std::string predict(const UserProfile& userProfile, const DataProcessor& processor) const;

private:
    RandomForest* m_randomForestModel;
};

#endif // STROKE_RISK_PREDICTOR_H